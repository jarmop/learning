import AddTask from './AddTask.js'
import TaskList from './TaskList.js'
import { TasksProvider } from './TasksProvider.js'

export default function TaskApp() {
  return (
    <TasksProvider>
      <h1>Prague itinerary</h1>
      <AddTask />
      <TaskList />
    </TasksProvider>
  )
}
