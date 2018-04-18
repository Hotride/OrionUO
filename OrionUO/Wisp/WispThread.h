//----------------------------------------------------------------------------------
#ifndef WISPTHREAD_H
#define WISPTHREAD_H
//----------------------------------------------------------------------------------
namespace WISP_THREAD
{
//----------------------------------------------------------------------------------
class CThread
{
private:
	HANDLE m_Handle{ 0 };
	UINT ID{ 0 };
	CRITICAL_SECTION m_CriticalSection;
	bool m_Paused{ false };
	bool m_Cycled{ false };
	int m_Delay{ 1 };
	bool m_Active{ false };

public:
	CThread();
	virtual ~CThread();

	void Run(bool cycled = false, int delay = 1, bool synchronizedDelay = false);

	bool IsActive();
	void Stop();
	bool Cycled();

	void Pause();
	void Resume();
	bool Paused();

	int Delay();
	void ChangeDelay(int newDelay);

	virtual void OnExecute(uint nowTime) {}
	virtual void OnDestroy() {}
};
//----------------------------------------------------------------------------------
}; //namespace
//----------------------------------------------------------------------------------
#endif

