/***************************************************************************
*
* Copyright 2014 Bruno Ordozgoiti
* Distributed under GNU GPLv3
*
* This file is part of GLandbox.
*
* GLandbox is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* GLandbox is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with GLandbox. If not, see <http://www.gnu.org/licenses/>.
*
***************************************************************************/

#include "Param.h"

namespace gland{

  void Param::operator =(const float f){    
    u.f = f;
    member_ = 0;
  }

  void Param::operator +=(const float f){    
    u.f += f;
    member_ = 0;
  }

  void Param::operator -=(const float f){    
    u.f -= f;
    member_ = 0;
  }

    
  void Param::operator =(int i){    
    u.i = i;
    member_ = 1;
  }

  void Param::operator +=(int i){    
    u.i += i;
    member_ = 1;
  }

  void Param::operator -=(int i){    
    u.i -= i;
    member_ = 1;
  }

    
  void Param::operator =(double d){    
    u.d = d;
    member_ = 2;
  }

  void Param::operator +=(double d){    
    u.d += d;
    member_ = 2;
  }

  void Param::operator -=(double d){    
    u.d -= d;
    member_ = 2;
  }


  void Param::operator =(bool b){    
    u.b = b;
    member_ = 4;
  }
  
  const unsigned short Param::getMember() const{
    return member_;
  }
                 
  
  std::ostream& operator << (std::ostream& stream, const gland::Param& p) {
    switch (p.getMember()){
    case 0:
      return stream << p.u.f;
      break;    
    case 1:
      return stream << p.u.i;
      break;
    case 2:
      return stream << p.u.d;
      break;
    case 3:
      //return stream << p.u.s;
      break;
    case 4:
      return stream << p.u.b;
      break;
    default:
      break;
    }

  }

  float operator *(const float f, const gland::Param &p)  {
    return p.u.f * f;
  }  

  int operator *(const int i, const gland::Param &p)  {
    return p.u.i * i;
  }  

  double operator *(const double d, const gland::Param &p)  {
    return p.u.d * d;
  }  


}
