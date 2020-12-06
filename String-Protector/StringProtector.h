#pragma once
#include <type_traits>
#include <cstdarg>

/*
	Notes:
	If the Decryption doesn't work try switching from Debug to Release mode
*/


// Removes references from class and makes it non-const
template<class Class>
using NoType = typename std::remove_const_t<std::remove_reference_t<Class>>;

template <unsigned long StringSize, int Key, int Seed, typename Type>
class SecureString
{
public:

	/*
		SecureString()
		Constructore executes at Compile-Time 
	*/
	__forceinline constexpr SecureString(Type* Data)
	{
		XORonCompile(Data);
	}

	/*
		Decrypt()
		Returns a decrypted copy of the string
		by making a copy of DataBuffer and temporarly decrypting the string
		We cant re-use the XORonCompile because that would permanantly decrypt DataBuffer
	*/
	__forceinline constexpr Type* Decrypt()
	{
		Type TempBuffer[StringSize]{};
		for (int i = 0; i < StringSize; i++)
		{
			TempBuffer[i] = DataBuffer[i] ^ (((Key + i + 1) * (i + 1)) * Seed);
		}
		return TempBuffer;
	}
	__forceinline operator Type* ()
	{
		return Decrypt();
	}

	/*
		Free()
		Delets the Encrypted String
	*/
	__forceinline void Free()
	{
		for (size_t i = 0; i < StringSize; i++)
		{
			DataBuffer[i] = 0;
		}
	}
	/*
		GetRaw()
		Return the encrypted string
	*/
	__forceinline Type* GetRaw()
	{
		return DataBuffer;
	}
private:
	/*
		XORonCompile()
		Encrypts string at Compile-Time
		We use a extra seed so that every encryption key in each iteration will be unique
	*/
	__forceinline constexpr void XORonCompile(Type* Data)
	{
        for (int i = 0; i < StringSize; i++)
        {
			DataBuffer[i] = Data[i] ^ (((Key + i + 1) * (i + 1)) * Seed);
        }
	}

	// Here is the Encrypted String saved
	Type DataBuffer[StringSize]{};
};

#define SString_Key(String, Key, Seed) []() {\
						constexpr static auto x = SecureString<sizeof(String) / sizeof(String[0]), Key, Seed, NoType<decltype(String[0])>>((NoType<decltype(String[0])>*)String);\
						return x;}()
#define SString(String) SString_Key(String,__TIME__[0], __TIME__[3])

/*
	MIT License

	Copyright(c) 2020 Fiereu

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this softwareand associated documentation files(the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and /or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions :

	The above copyright noticeand this permission notice shall be included in all
	copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.
*/