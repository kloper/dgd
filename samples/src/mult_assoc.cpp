// associate multiple channels with cout

#include <dgChannel.h>

int main( int argc, char** argv ) {
   DGD::channel base( "base" );
   DGD::assoc( std::cout, base );
   base.open();

   DGD::channel a( "channel_a" );
   DGD::assoc( base, a );
   a.open();

   DGD::channel b( "channel_b" );
   DGD::assoc( base, b );
   b.open();
   
   a << "here is channel A" << std::endl;
   b << "here is channel B" << std::endl;

   return 0;
}
