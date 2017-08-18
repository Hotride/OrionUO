//----------------------------------------------------------------------------------
#ifndef WISPBINARYFILEWRITTER_H
#define WISPBINARYFILEWRITTER_H
//----------------------------------------------------------------------------------
namespace WISP_FILE
{
//----------------------------------------------------------------------------------
class CBinaryFileWritter : public WISP_DATASTREAM::CDataWritter
{
private:
	FILE *m_File{ NULL };

public:
	CBinaryFileWritter();

	virtual ~CBinaryFileWritter();

	bool Open(const string &path);
	bool Open(const wstring &path);

	void Close();

	void WriteBuffer();
};
//----------------------------------------------------------------------------------
}; //namespace
//----------------------------------------------------------------------------------
#endif

