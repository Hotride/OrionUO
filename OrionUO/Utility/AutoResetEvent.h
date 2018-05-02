//----------------------------------------------------------------------------------
#ifndef AUTORESETEVENT_H
#define AUTORESETEVENT_H
//----------------------------------------------------------------------------------
//http://stackoverflow.com/questions/8538575/is-there-an-easy-way-to-implement-autoresetevent-in-c0x
//Этим обьектом можно сигналить между тредами.
//Тред А будет ждать пока Тред Б посигналит через этот обьект об окончании своей работы.
///
class AutoResetEvent
{
public:
    explicit AutoResetEvent(bool initial = false);

    void Set();
    void Reset();

    bool WaitOne();

private:
    AutoResetEvent(const AutoResetEvent &);
    AutoResetEvent &operator=(const AutoResetEvent &); // non-copyable
    bool flag_;
    std::mutex protect_;
    std::condition_variable signal_;
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
