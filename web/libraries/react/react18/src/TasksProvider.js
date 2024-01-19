import { useTasksReducer } from './tasksReducer'
import { TasksContext, TasksDispatchContext } from './TasksContext'

export function TasksProvider({ children }) {
  const [tasks, dispatch] = useTasksReducer()

  return (
    <TasksContext.Provider value={tasks}>
      <TasksDispatchContext.Provider value={dispatch}>{children}</TasksDispatchContext.Provider>
    </TasksContext.Provider>
  )
}
