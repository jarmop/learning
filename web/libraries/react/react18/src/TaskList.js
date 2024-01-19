import { useContext, useState } from 'react'
import { TasksContext, TasksDispatchContext } from './TasksContext'
import { changeTask, deleteTask } from './tasksReducer'

export default function TaskList() {
  const tasks = useContext(TasksContext)

  return (
    <ul>
      {tasks.map((task) => (
        <li key={task.id}>
          <Task task={task} />
        </li>
      ))}
    </ul>
  )
}

function Task({ task, onChange, onDelete }) {
  const [isEditing, setIsEditing] = useState(false)
  const dispatch = useContext(TasksDispatchContext)

  let taskContent
  if (isEditing) {
    taskContent = (
      <>
        <input
          value={task.text}
          onChange={(e) =>
            dispatch(changeTask({
              ...task,
              text: e.target.value,
            }))
          }
        />
        <button onClick={() => setIsEditing(false)}>Save</button>
      </>
    )
  } else {
    taskContent = (
      <>
        {task.text}
        <button onClick={() => setIsEditing(true)}>Edit</button>
      </>
    )
  }
  return (
    <label>
      <input
        type="checkbox"
        checked={task.done}
        onChange={(e) =>
          dispatch(changeTask({
            ...task,
            done: e.target.checked,
          }))
        }
      />
      {taskContent}
      <button onClick={() => dispatch(deleteTask(task.id))}>Delete</button>
    </label>
  )
}
