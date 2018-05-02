#include "StackWalker.h"
#include "Wisp/WispLogger.h"

class OrionStackWalker : public StackWalker
{
public:
    OrionStackWalker()
        : StackWalker()
    {
    }

protected:
    virtual void OnOutput(LPCSTR szText)
    {
        CRASHLOG(szText);
        StackWalker::OnOutput(szText);
    }
};
