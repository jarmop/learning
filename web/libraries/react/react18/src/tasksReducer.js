import { useReducer } from 'react'

export function addTask(text) {
  return {
    type: 'added',
    id: nextId++,
    text: text,
  }
}

export function changeTask(task) {
  return {
    type: 'changed',
    task: task,
  }
}

export function deleteTask(taskId) {
  return { type: 'deleted', id: taskId }
}

function tasksReducer(tasks, action) {
  switch (action.type) {
    case 'added': {
      return [
        ...tasks,
        {
          id: action.id,
          text: action.text,
          done: false,
        },
      ]
    }
    case 'changed': {
      return tasks.map((task) => {
        if (task.id === action.task.id) {
          return action.task
        } else {
          return task
        }
      })
    }
    case 'deleted': {
      return tasks.filter((t) => t.id !== action.id)
    }
    default: {
      throw Error('Unknown action: ' + action.type)
    }
  }
}

export function useTasksReducer() {
  return useReducer(tasksReducer, initialTasks)
}

let nextId = 3
const initialTasks = [
  { id: 0, text: 'Visit Kafka Museum', done: true },
  { id: 1, text: 'Watch a puppet show', done: false },
  { id: 2, text: 'Lennon Wall pic', done: false },
]
