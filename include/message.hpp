#ifndef MESSAGE_HPP
#define MESSAGE_HPP

struct Message
{
  int what;
  int arg1;
  int arg2;
  void* obj; // C++

//  bool operator== ( Message & arg_msg ) const
//  {
//    const bool loc_cond1 = ( what == arg_msg.what );
//    const bool loc_cond2 = ( arg1 == arg_msg.arg1 );
//    const bool loc_cond3 = ( arg2 == arg_msg.arg2 );
//    const bool loc_cond4 = ( obj == arg_msg.obj );
//
//    return ( loc_cond1 && loc_cond2 && loc_cond3 && loc_cond4 );
//  }

  bool operator!= ( const Message  &arg_msg ) const
  {
    const bool loc_cond1 = ( what == arg_msg.what );
    const bool loc_cond2 = ( arg1 == arg_msg.arg1 );
    const bool loc_cond3 = ( arg2 == arg_msg.arg2 );
    const bool loc_cond4 = ( obj == arg_msg.obj );

    return ( loc_cond1 && loc_cond2 && loc_cond3 && loc_cond4 ) ? false : true;
  }
};

#endif
