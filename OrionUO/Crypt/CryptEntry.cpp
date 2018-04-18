//----------------------------------------------------------------------------------
#if !USE_ORIONDLL
#include "../Wisp/WispGlobal.h"
#include "../Wisp/WispDataStream.h"
#include "../EnumList.h"
#include "../PluginInterface.h"
#include "LoginCrypt.h"
#include "GameCrypt.h"
//----------------------------------------------------------------------------------
ENCRYPTION_TYPE g_EncryptionType = ET_NOCRYPT;
size_t g_CryptPluginsCount = 0;
UCHAR_LIST g_RawData;
//----------------------------------------------------------------------------------
static void Init(const bool &mode, u8 *array)
{
	if (mode)
		g_LoginCrypt.Init(array);
	else
	{
		if (g_EncryptionType != ET_NOCRYPT)
			g_BlowfishCrypt.Init();

		if (g_EncryptionType == ET_203 || g_EncryptionType == ET_TFISH)
		{
			g_TwofishCrypt.Init(array);

			if (g_EncryptionType == ET_TFISH)
				g_TwofishCrypt.Init_MD5();
		}
	}
}
//----------------------------------------------------------------------------------
static void Send(const bool &mode, u8 *src, u8 *dest, const int &size)
{
	if (g_EncryptionType == ET_NOCRYPT)
		memcpy(&dest[0], &src[0], size);
	else if (mode)
	{
		if (g_EncryptionType == ET_OLD_BFISH)
			g_LoginCrypt.Encrypt_Old(&src[0], &dest[0], size);
		else if (g_EncryptionType == ET_1_25_36)
			g_LoginCrypt.Encrypt_1_25_36(&src[0], &dest[0], size);
		else if (g_EncryptionType != ET_NOCRYPT)
			g_LoginCrypt.Encrypt(&src[0], &dest[0], size);
	}
	else if (g_EncryptionType == ET_203)
	{
		g_BlowfishCrypt.Encrypt(&src[0], &dest[0], size);
		g_TwofishCrypt.Encrypt(&dest[0], &dest[0], size);
	}
	else if (g_EncryptionType == ET_TFISH)
		g_TwofishCrypt.Encrypt(&src[0], &dest[0], size);
	else
		g_BlowfishCrypt.Encrypt(&src[0], &dest[0], size);
}
//----------------------------------------------------------------------------------
static void Decrypt(u8 *src, u8 *dest, const int &size)
{
	if (g_EncryptionType == ET_TFISH)
		g_TwofishCrypt.Decrypt(&src[0], &dest[0], size);
	else
		memcpy(&dest[0], &src[0], size);
}
//----------------------------------------------------------------------------------
static void LoadPlugins(PLUGIN_INFO *result)
{
	if (g_RawData.size())
	{
		WISP_DATASTREAM::CDataReader file(&g_RawData[0], (int)g_RawData.size());

		uchar ver = file.ReadUInt8();

		file.Move(2);
		int len = file.ReadUInt8();
		file.Move(len + 39);

		if (ver >= 2)
		{
			file.Move(1);

			if (ver >= 3)
				file.Move(1);

			char count = file.ReadInt8();

			IFOR(i, 0, count)
			{
				short len = file.ReadInt16LE();
				string fileName = file.ReadString(len);
				memcpy(&result[i].FileName[0], &fileName.data()[0], fileName.length());

				file.Move(2);
				result[i].Flags = file.ReadUInt32LE();
				file.Move(2);

				len = file.ReadInt16LE();
				string functionName = file.ReadString(len);
				memcpy(&result[i].FunctionName[0], &functionName.data()[0], functionName.length());
			}
		}
	}
}
//----------------------------------------------------------------------------------
UCHAR_LIST ApplyInstall(uchar *address, size_t size)
{
	UCHAR_LIST result;

	if (size)
	{
		g_RawData.resize(size);
		memcpy(&g_RawData[0], &address[0], size);

		WISP_DATASTREAM::CDataReader file(address, size);
		WISP_DATASTREAM::CDataWritter writter;

		uchar version = file.ReadInt8();
		writter.WriteUInt8(version);
		writter.WriteUInt8(0xFE); //dll version
		writter.WriteUInt8(0); //sub version

		g_EncryptionType = (ENCRYPTION_TYPE)file.ReadInt8();
		writter.WriteUInt8(file.ReadInt8()); //ClientVersion

		int len = file.ReadInt8();
		writter.WriteUInt8(len);
		writter.WriteString(file.ReadString(len), len, false);

		file.Move(14); //crypt keys & seed
#if defined(_M_IX86)
		writter.WriteUInt32LE((size_t)Init);
		writter.WriteUInt32LE((size_t)Send);
		writter.WriteUInt32LE((size_t)Decrypt);
		writter.WriteUInt32LE((size_t)LoadPlugins);
#else
		writter.WriteUInt64LE((size_t)Init);
		writter.WriteUInt64LE((size_t)Send);
		writter.WriteUInt64LE((size_t)Decrypt);
		writter.WriteUInt64LE((size_t)LoadPlugins);
#endif

		int mapsCount = 6;

		if (version < 4)
			writter.WriteUInt8(file.ReadInt8()); //InverseBuylist
		else
		{
			mapsCount = file.ReadInt8();
			writter.WriteUInt8(mapsCount);
		}

		IFOR(i, 0, mapsCount)
		{
			writter.WriteUInt16LE(file.ReadUInt16LE());
			writter.WriteUInt16LE(file.ReadUInt16LE());
		}

		uchar clientFlag = 0;
		uchar useVerdata = 0;

		if (version >= 2)
		{
			clientFlag = file.ReadInt8();

			if (version >= 3)
				useVerdata = file.ReadInt8();

			g_CryptPluginsCount = file.ReadInt8();
		}

		writter.WriteUInt8(clientFlag);
		writter.WriteUInt8(useVerdata);

		result = writter.Data();
	}

	//ParseCommandLine();

	return result;
}
//----------------------------------------------------------------------------------
size_t GetPluginsCount()
{
	return g_CryptPluginsCount;
}
//----------------------------------------------------------------------------------
void CryptInstallNew(uchar *address, size_t size, uchar *result, size_t &resultSize)
{
	assert(address && result && size && resultSize );
	UCHAR_LIST buf = ApplyInstall(address, size);
	memset(result, 0, resultSize);
	resultSize = buf.size();
	if (resultSize >= buf.size())
		memcpy(result, &buf[0], buf.size());
}
#endif

