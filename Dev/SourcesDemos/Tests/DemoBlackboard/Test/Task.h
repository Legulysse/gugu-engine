////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/System/String.h"

#include <coroutine>

////////////////////////////////////////////////////////////////
// File Declarations

namespace test {

template<typename TAbortContext>
class TaskA
{
public:

    virtual void Abort(const TAbortContext& context) {}
};

template<typename TAbortContext>
class ManagerTasksA
{
public:

    void PushTask(TaskA<TAbortContext>* task)
    {
        m_tasks.push_back(task);
    }

    void AbortAllTasks(const TAbortContext& context)
    {
        for (TaskA<TAbortContext>* task : m_tasks)
        {
            task->Abort(context);
        }
    }

protected:

    std::vector<TaskA<TAbortContext>*> m_tasks;
};

enum class EAbortReasonA
{
    Cancel,
    Kill,
};

class TaskCharacterMoveA :public TaskA<EAbortReasonA>
{
public:

    virtual void Abort(const EAbortReasonA& context) override
    {
    }
};

//----------------------------------------------

class TaskB
{
public:

    virtual void Abort(const gugu::uint32& abortFlags) {}
};

class ManagerTasksB
{
public:

    void PushTask(TaskB* task)
    {
        m_tasks.push_back(task);
    }

    void AbortAllTasks(const gugu::uint32& abortFlags)
    {
        for (TaskB* task : m_tasks)
        {
            task->Abort(abortFlags);
        }
    }

protected:

    std::vector<TaskB*> m_tasks;
};

enum class EAbortReasonB : gugu::uint32
{
    Cancel,
    Kill,
};

class TaskCharacterMoveB :public TaskB
{
public:

    virtual void Abort(const gugu::uint32& abortFlags) override
    {
        EAbortReasonB abortReason = (EAbortReasonB)abortFlags;
    }
};

//----------------------------------------------

struct CustomPromise;
struct CustomCoroutine : std::coroutine_handle<CustomPromise>
{
    using promise_type = test::CustomPromise;
};
struct CustomPromise
{
    // TODO:
    // - Check meanings for std::suspend_always vs std::suspend_never.
    CustomCoroutine get_return_object() { return { CustomCoroutine::from_promise(*this) }; }
    std::suspend_always initial_suspend() noexcept { return {}; }
    std::suspend_always final_suspend() noexcept { return {}; }
    std::suspend_always yield_value(nullptr_t x) { return {}; }
    //std::suspend_always yield_value(CustomCoroutine x) { return {}; }
    void return_void() {}
    void unhandled_exception() {}
};

#define YIELD_RETURN_NULL co_yield 0
//#define YIELD_RETURN_NULL co_yield nullptr
//#define YIELD_RETURN_NULL co_yield {}


struct CustomSubroutineAwaiter
{
public:
    CustomSubroutineAwaiter(CustomCoroutine subroutine) : m_subroutine(subroutine) {}
    ~CustomSubroutineAwaiter() { m_subroutine.destroy(); }
    bool await_ready() const noexcept { return false; /* !(m_subroutine.operator bool()) || m_subroutine.done();*/ }
    void await_resume() noexcept { /*m_subroutine.resume();*/ }
    //bool await_resume() noexcept { m_subroutine.resume(); }
    //bool await_resume() noexcept { m_subroutine.resume(); return !m_subroutine.done(); }
    //void await_suspend(std::coroutine_handle<> h) noexcept { }
    std::coroutine_handle<> await_suspend(std::coroutine_handle<> h) noexcept { return m_subroutine; }
private:
    CustomCoroutine m_subroutine;
};



CustomCoroutine FreeFunctionC()
{
    int localValue = 0;
    gugu::WriteInIDEConsoleEndline(gugu::StringFormat("Step {0}", localValue++));
    YIELD_RETURN_NULL;
    gugu::WriteInIDEConsoleEndline(gugu::StringFormat("Step {0}", localValue++));
    YIELD_RETURN_NULL;
    // nothing
    YIELD_RETURN_NULL;
    // nothing
    YIELD_RETURN_NULL;
    localValue++;
    YIELD_RETURN_NULL;
    localValue++;
    YIELD_RETURN_NULL;
    gugu::WriteInIDEConsoleEndline(gugu::StringFormat("Step {0}", localValue++));
    co_return;
}

CustomCoroutine WaitForTicks(int ticks)
{
    while (ticks > 0)
    {
        ticks--;
        gugu::WriteInIDEConsoleEndline(gugu::StringFormat("Wait Ticks {0}", ticks));
        //co_await std::suspend_always{};
        YIELD_RETURN_NULL;
    }
    co_return;
}

auto Awaiter_WaitForTicks(int ticks)
{
    return CustomSubroutineAwaiter{ WaitForTicks(ticks) };
}

class TaskC
{
public:
    void Begin()                { m_coroutine = RunImpl(); }
    bool IsInProgress() const   { return !m_coroutine.done(); }
    void Progress()             { if (!m_coroutine.done()) m_coroutine.resume(); }
    void Finalize()             { m_coroutine.destroy(); }
protected:
    virtual CustomCoroutine RunImpl() = 0;
private:
    CustomCoroutine m_coroutine;
};

class TaskC_CharacterMove : public TaskC
{
public:
    TaskC_CharacterMove(const std::string& name) : m_name(name) {}
protected:
    virtual CustomCoroutine RunImpl() override
    {
        int localValue = 0;

        ++m_memberValue; ++localValue;
        gugu::WriteInIDEConsoleEndline(gugu::StringFormat("Step {0} {1} {2}", m_name, m_memberValue, localValue));
        YIELD_RETURN_NULL;

        ++m_memberValue; ++localValue;
        gugu::WriteInIDEConsoleEndline(gugu::StringFormat("Step {0} {1} {2}", m_name, m_memberValue, localValue));
        YIELD_RETURN_NULL;

        // Manual subroutine
        gugu::WriteInIDEConsoleEndline(gugu::StringFormat("Manual subroutine start {0}", m_name));
        CustomCoroutine subroutine;
        subroutine = WaitForTicks(4);
        while (!subroutine.done())
        {
            subroutine.resume();
            YIELD_RETURN_NULL;
        }
        subroutine.destroy();

        // Wrapped subroutine
        gugu::WriteInIDEConsoleEndline(gugu::StringFormat("Wrapped subroutine start (A) {0}", m_name));
        co_await CustomSubroutineAwaiter(WaitForTicks(4));
        gugu::WriteInIDEConsoleEndline(gugu::StringFormat("Wrapped subroutine start (B) {0}", m_name));
        co_await Awaiter_WaitForTicks(4);

        ++m_memberValue; ++localValue;
        gugu::WriteInIDEConsoleEndline(gugu::StringFormat("Step {0} {1} {2}", m_name, m_memberValue, localValue));
        co_return;
    }
    std::string m_name;
    int m_memberValue = 0;
};

//----------------------------------------------

void RunTestTask()
{
    using namespace gugu;

    //----------------------------------------------
    {
        ManagerTasksA<EAbortReasonA>* managerTasks = new ManagerTasksA<EAbortReasonA>;

        TaskCharacterMoveA* taskMove = new TaskCharacterMoveA;
        managerTasks->PushTask(taskMove);
        managerTasks->AbortAllTasks(EAbortReasonA::Cancel);

        SafeDelete(taskMove);
        SafeDelete(managerTasks);
    }
    //----------------------------------------------
    {
        ManagerTasksB* managerTasks = new ManagerTasksB;

        TaskCharacterMoveB* taskMove = new TaskCharacterMoveB;
        managerTasks->PushTask(taskMove);
        managerTasks->AbortAllTasks((uint32)EAbortReasonB::Cancel);

        SafeDelete(taskMove);
        SafeDelete(managerTasks);
    }
    //----------------------------------------------
    {
        CustomCoroutine routine = FreeFunctionC();
        while (!routine.done())
        {
            routine.resume();
        }
        routine.destroy();

        auto task1 = new TaskC_CharacterMove("Billy");
        auto task2 = new TaskC_CharacterMove("Paula");
        task1->Begin();
        task2->Begin();
        while (task1->IsInProgress() && task2->IsInProgress())
        {
            task1->Progress();
            task2->Progress();
        }
        task1->Finalize();
        task2->Finalize();

        SafeDelete(task1);
        SafeDelete(task2);
    }
}

}   //namespace test
