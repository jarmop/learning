import React, {Component} from 'react';
import './App.css';
import { connect } from 'react-redux'

let nextTodoId = 2;
export const addTodo = (text) => ({
    type: 'ADD_TODO',
    id: nextTodoId++,
    text
});

export const removeTodo = (id) => ({
    type: 'REMOVE_TODO',
    id
});

export const myReducer = (state = [], action) => {
    switch (action.type) {
        case 'ADD_TODO':
            return {todos: [
                ...state.todos,
                {
                    id: action.id,
                    text: action.text
                }
            ]};
        case 'REMOVE_TODO':
            return {
                todos: state.todos.filter(todo => todo.id !== action.id)
            };
        default:
            return state
    }
};

export let App = ({dispatch, todos}) => {
    let input;

    return (
        <div>
            <form onSubmit={e => {
                e.preventDefault();
                if (!input.value.trim()) {
                    return
                }
                dispatch(addTodo(input.value));
                input.value = ''
            }}>
                <input ref={node => {
                    input = node
                }} />
                <button type="submit">
                    Add Todo
                </button>
            </form>
            <ul>
                {todos.map((todo) =>
                    <li key={todo.id}>
                        {todo.text}
                        <button onClick={() => dispatch(removeTodo(todo.id))}>X</button>
                    </li>
                )}
            </ul>
        </div>
    )
};

class TodoForm extends Component {
    constructor(props) {
        super(props);
        this.state = {
            value: 'do it!'
        }
    }

    onValueChange(event) {
        this.setState({value: event.target.value});
    }

    onSubmit(event) {
        event.preventDefault();
        this.props.onSubmit(this.state.value);
    }

    render() {
        return (
            <form onSubmit={this.onSubmit.bind(this)}>
                {/*<input type="text" value={this.state.value} onChange={this.onValueChange.bind(this)}/>*/}
                <input type="text" value={this.state.value} onChange={this.onValueChange.bind(this)}/>
                <input type="submit" value="Add todo"/>
            </form>
        );
    }
}

const TodoList = (props) => {
    return (
        <ul>
            {props.todos.map((todo, index) =>
                <Todo key={index} index={index} text={todo} onRemove={props.onRemoveTodo.bind(this)}/>
            )}
        </ul>
    );
};

const Todo = (props) => {
    return (
        <li>
            {props.text}
            {' '}
            <button onClick={() => props.onRemove(props.index)}>X</button>
        </li>
    );
};

// export default App;

const mapStateToProps = (state) => ({
    todos: state.todos
});

App = connect(mapStateToProps)(App);


