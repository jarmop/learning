import { useContext, useState } from 'react'
import { TasksDispatchContext } from './TasksContext'
import { addTask } from './tasksReducer'

export default function AddTask({ onAddTask }) {
  const [text, setText] = useState('')
  const dispatch = useContext(TasksDispatchContext)

  return (
    <>
      <input placeholder="Add task" value={text} onChange={(e) => setText(e.target.value)} />
      <button
        onClick={() => {
          setText('')
          dispatch(addTask(text))
        }}
      >
        Add
      </button>
    </>
  )
}

