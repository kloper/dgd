// format output with channelbuf

#include <dgChannel.h>
#include <dgStream.h>

main( int argc, char** argv ) {
   DGD::channel formatter( "formatter" );

   DGD::assoc( &std::cout, formatter );
   formatter.open();

   formatter << "Hello! This is actally very very very " 
	     << "very very very very very very very very " 
	     << "very very very very very very very long line!" 
	     << "longer then " << formatter.max_width() << "!"
	     << std::endl;

   formatter.max_width( 40 );

   formatter << "Hello! This is another very very very " 
	     << "very very very very very very very very " 
	     << "very very very very very very very long line!" 
	     << "longer then " << formatter.max_width() << "!"
	     << std::endl;

   formatter.word_wrap( false );

   formatter << "Hello! This is another very very very " 
	     << "very very very very very very very very " 
	     << "very very very very very very very long line!" 
	     << "longer then " << formatter.max_width() << "!"
	     << std::endl;

   formatter.word_wrap( true );

   formatter.space_chars(" \t;");
   formatter.max_width( 4 );
   formatter << "a;b;c;d;e;f;g;h" << std::endl;

   formatter.max_width( 60 );
   formatter.space_chars(" \t");
   
   for( int i = 0; i < 76; i++ ) {
      formatter << "word ";
      if( i > 0 && i % 15 == 0 ) {
	 formatter << std::endl;
	 if( i < 32 )
	    formatter.incr_indent();
	 else 
	    formatter.decr_indent();
      }
   }

   return 0;
}
