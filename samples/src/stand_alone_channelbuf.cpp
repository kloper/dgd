// format output with channelbuf

#include <dgChannelBuf.h>
#include <dgStream.h>

main( int argc, char** argv ) {
   DGD::channelbuf formatter;

   formatter.pubsetbuf( NULL, 100 );
   formatter.assoc( &std::cout );

   std::ostream formatting_stream( &formatter );

   formatting_stream << "Hello! This is actally very very very " 
		     << "very very very very very very very very " 
		     << "very very very very very very very long line!" 
		     << std::endl;
   return 0;
}
