#include <iostream>
#include <list>

#include <dgDebug.h>

using namespace DGD;

struct MyStruct {
      int my_field;
      
      MyStruct( int val ): my_field( val ) {}
};

std::ostream& operator << ( std::ostream& ostr, const MyStruct& data ) {
   ostr << "[" << data.my_field << "]";
   return ostr;
}

int main( int argc, char** argv ) {
   std::list<MyStruct> my_data;
   int i;
   
   Debug::debug_factory_ref dout = Debug::create_factory( argc, argv );

   for( i = 0; i < 10; i++ ) my_data.push_back( i );

   dgd_trace( main, dgd_expand(my_data) << std::endl );
   
   return 0;
}
