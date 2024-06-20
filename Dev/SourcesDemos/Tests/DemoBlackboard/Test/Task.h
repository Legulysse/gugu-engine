////////////////////////////////////////////////////////////////
// Includes


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
}

}   //namespace test
