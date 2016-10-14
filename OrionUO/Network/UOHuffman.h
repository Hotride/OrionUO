//---------------------------------------------------------------------------
#ifndef UOHuffmanH
#define UOHuffmanH
//---------------------------------------------------------------------------
class CDecompressingCopier
{
private:
	static int tree[512];
	int value;
	int mask;
	int bit_num;
	int treepos;

public:
	CDecompressingCopier() {initialise();}

	void initialise() {bit_num = 8; treepos = 0;}

	virtual void operator () (char * dest, const char * src, int & dest_size, int & src_size);
};
//---------------------------------------------------------------------------
#endif
