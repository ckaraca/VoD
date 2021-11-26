#pragma once
// taken from http://www.createwindow.com/programming/crc32/index.htm
class CCRC
{
public:
	CCRC(void);
	~CCRC(void);
protected: 
      ULONG crc32_table[256];  // Lookup table array 
     
      ULONG Reflect(ULONG ref, char ch);  // Reflects CRC bits in the lookup table 
public:
	void Init_CRC32_Table();  // Builds lookup table array 
    int Get_CRC(char* text);  // Creates a CRC from a text string 
	//int Get_CRC(CString& text);  // Creates a CRC from a text string 

};
