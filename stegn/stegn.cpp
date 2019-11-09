uint8 EncodeChannel( uint8 ChannelValue )
{
   uint8 BitData;
   uint Count;

   // Make sure we haven't pass the end of the message
   // Note that in the event we're pass the message length, we don't modify
   // the data at all
   if ( MessageIndex < MessageLength )
   {
     // Start with no bit data
     BitData = 0;

     // For each bit of data to encode into the channel (specified by the 
     // user)
     for ( Count = 0; Count < BitsPerChannel; ++Count )
     {
       // Shift the current message to the left by one
       BitData <<= 1;

       // Add the MSB from the current message to the bit data
       BitData |= ( CurrentMessageData & BIT7 ) >> 7;

       // Shift the message data to the left by one
       CurrentMessageData <<= 1;

       // Count that we've added this bit from the message
       --CurrentMessageBits;

       // Out of bits in this character?
       if ( CurrentMessageBits == 0 )
       {
         // Get a new character (if we still have characters left)
         if ( MessageIndex < MessageLength )
         {
           ++MessageIndex;
           CurrentMessageData = Message[ MessageIndex ];
         }
         else
           CurrentMessageData = 0; // <- End of message, just pad with nothing

         // We now have 8-bits of new data
         CurrentMessageBits += 8;
       }

     }

     // Loss orignal bits
     ChannelValue &= BitsPerChannelMask;

     // Add in new bits
     ChannelValue |= BitData;
   }

   return ChannelValue;
}

/---------------------------------------------------------------------------
// Encode 'DataFile' into 'ImageFile' and place output in 'OutputFile'
//---------------------------------------------------------------------------
void EncodeImageIntoFile( 
  char const * InputFileName , 
  char const * OutputFileName ,
  char const * DataFileName )
{
   PNG_ImageClass PNG_Image;
   bool Result;
   uint ImageCapacity;  // <- In bits

   //----------------------------------
   // Load input image
   //----------------------------------
   Result = PNG_Image.LoadPNG( InputFileName );
   if ( ! Result )
     throw GeneralError( "Unable to load PNG image: %s\n" , PNG_Image.ErrorString );

   // Compute image capacity (in bits)
   ImageCapacity = PNG_Image.Height * PNG_Image.Width * 3 * BitsPerChannel;

   //----------------------------------
   // Load data file
   //----------------------------------
   FILE*  DataFile;
   uint8* MessageData;
   uint   MessageSize; // <- In bits

   // Try and open data file
   DataFile = fopen( DataFileName , "rb" );

   // Problems?
   if ( ! DataFile )
     throw GeneralError( "Unable to open '%s'\n" , DataFileName );

   // Get file size
   fseek( DataFile , 0 , SEEK_END );
   MessageLength = ftell( DataFile );
   rewind( DataFile );

   // How much data in message?
   // This is the file size (MessageLength) and header size, both in bits
   MessageSize = ( MessageLength * 8 ) + ( sizeof( *ImageHeader ) * 8 );

   if ( IsVerbose )
     printf( "Message size/image capacity (in bits): %i/%i\n" , 
       MessageSize , ImageCapacity );

   // Too much information for image?
   if ( MessageSize > ImageCapacity )
     throw GeneralError( "Data too large to fit into image\nEither select a larger image or encode less data\nThis image can hold %i bytes of data, data file is %i bytes"  , ImageCapacity / 8 , MessageLength );

   // Allocate memory to contain the whole file.
   Message = (uint8*)malloc( MessageLength + sizeof( *ImageHeader ) );
   if ( ! Message )
     throw GeneralError( "Unable to allocate memory for data file (maybe it's too big)" );

   // Set header & message location
   ImageHeader = (ImageFullHeader*)&Message[ 0 ];
   MessageData = (uint8*)&Message[ sizeof( *ImageHeader ) ];

   // Set message size
   ImageHeader->EncryptedHeader.MessageSize = MessageLength;

   // Load file into buffer
   fread( MessageData , 1 , MessageLength , DataFile );

   // Finished with data file
   fclose( DataFile );

   //----------------------------------
   // Compute MAC for message
   //----------------------------------

   // Start hash (for MAC)
   HashClass Hash( SelectedMAC );

   // Start MAC hash
   Hash.Initilize();

   // Hash data
   Hash.AddData( MessageData , MessageLength );

   // Finilize MAC hash
   Hash.Finilize();

   // Copy hash to header
   memcpy( 
     ImageHeader->EncryptedHeader.MAC , 
     Hash.GetHash() , 
     (size_t)SelectedMAC_Size );

   // Display message hash
   if ( IsVerbose )
   {
     char StringBuffer[ 256 ];
     printf( "Message hash..: %s\n" , 
       BufferToHex( StringBuffer , Hash.GetHash() , SelectedMAC_Size ) );
   }

   // Clean MAC hash
   // (Kinda redundant, since the destructor will do this)
   Hash.Flush();

   uint MessageDataLength = MessageLength;

   // Add header into message length
   MessageLength += sizeof( *ImageHeader );

   //----------------------------------
   // Encrypt data
   //----------------------------------
   uint8* Key;

   // Start key expantion
   KeyExpantionClass KeyExpantion( 
     SelectedHash , SelectedSaltSize , SelectedKeySize );

   // Start cipher
   CipherClass Cipher( 
     SelectedCipher , 
     SelectedCipherMode , 
     SelectedKeySize , 
     SelectedIV_Size );

   // Create key
   KeyExpantion.CreateNewExpandedKey( Passphrase , ImageHeader->Salt , &Key );

   // Generate IV
   Cipher.CreateIV( ImageHeader->IV );

   if ( IsVerbose )
   {
     char StringBuffer[ 256 ];
     printf( "Encrypt key...: %s \n" , 
       BufferToHex( StringBuffer , Key , SelectedKeySize ) );

     printf( "Salt..........: %s \n" , 
       BufferToHex( StringBuffer , ImageHeader->Salt , SelectedSaltSize ) );

     printf( "IV............: %s \n" , 
       BufferToHex( StringBuffer , ImageHeader->IV , SelectedIV_Size ) );
   }

   // Initilize cipher
   Cipher.InitilizeCipher( Key , SelectedKeySize , ImageHeader->IV );
   
   // Encrypt the encrypted header data
   size_t EncryptedHeaderSize = sizeof( ImageHeader->EncryptedHeader );
   Cipher.EncryptBuffer( 
     (uint8*)&ImageHeader->EncryptedHeader , 
     EncryptedHeaderSize );

   // Encrypt message
   Cipher.EncryptBuffer( MessageData , MessageDataLength );

   // Finish with cipher
   Cipher.CloseCipher();

   //----------------------------------
   // Encode data
   //----------------------------------
   MessageIndex       = 0;
   CurrentMessageData = Message[ 0 ];
   CurrentMessageBits = 8;
   BitsPerChannelMask = ~( ( 1 << BitsPerChannel ) - 1 );

   for ( uint y = 0; y < PNG_Image.Height; y++ ) 
     for ( uint x = 0; x < PNG_Image.Width; x++ ) 
       for ( uint Channel = 0; Channel < 3; Channel++ )
         PNG_Image.ImageData[ y ][ x * 3 + Channel ] = 
           EncodeChannel( PNG_Image.ImageData[ y ][ x * 3 + Channel ] );

   //----------------------------------
   // Save resulting image
   //----------------------------------
   Result = PNG_Image.SavePNG( OutputFileName );
   if ( ! Result )
     throw GeneralError( "Unable to save PNG image" );
}