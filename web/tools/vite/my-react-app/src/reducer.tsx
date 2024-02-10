import { replaceAtIndex } from "./util"

interface SetDataAction {
  type: 'setData'
  data: string[][]
}

interface ChangeRowAction {
  type: 'changeRow'
  row: string[]
  i: number
}

interface ChangeCellAction {
  type: 'changeCell'
  value: string
  i: number
  j: number
}

interface State {
  data: string[][]
}

export function reducer(state: State, action: SetDataAction | ChangeRowAction | ChangeCellAction) {
  switch (action.type) {
    case 'setData': {
      return { data: action.data }
    }
    case 'changeRow': {
      return { data: replaceAtIndex(state.data, action.i, action.row) }
    }
    case 'changeCell': {
      return {
        data: replaceAtIndex(state.data, action.i, replaceAtIndex(state.data[action.i], action.j, action.value)),
      }
    }
    default: {
      return state
    }
  }
}