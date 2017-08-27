import React, {Component} from 'react';
import './App.css';
import { connect } from 'react-redux'

let nextTodoId = 0;
export const addTodo = (text) => ({
    type: 'ADD_TODO',
    // id: nextTodoId++,
    text
});

export const toggleTodo = (id) => ({
    type: 'TOGGLE_TODO',
    id
});

export const myReducer = (state = [], action) => {
    console.log(state);
    switch (action.type) {
        case 'ADD_TODO':
            return {todos: [
                ...state.todos,
                action.text
            ]};
        case 'REMOVE_TODO':
            return state.map(todo =>
                (todo.id === action.id)
                    ? {...todo, completed: !todo.completed}
                    : todo
            );
        default:
            return state
    }
};


// export let App = ({dispatch}) => {
export let App = ({dispatch, todos}) => {
    // constructor(props) {
    //     super(props)
    //     this.state = {
    //         todos: [
    //             'first todo',
    //             'second todo',
    //         ]
    //     }
    // }

    // addTodo(text) {
    //     let todos = this.state.todos;
    //     todos.push(text);
    //     this.setState(todos);
    // }
    //
    // removeTodo(indexToRemove) {
    //     let todos = this.state.todos;
    //     todos = todos.filter((todo, index) => index !== indexToRemove);
    //     this.setState({'todos': todos});
    // }

    let input;

    return (
        <div>
            <form onSubmit={e => {
                e.preventDefault()
                if (!input.value.trim()) {
                    return
                }
                dispatch(addTodo(input.value))
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
                {todos.map((todo, index) =>
                    <li key={index}>{todo}</li>
                )}
            </ul>
        </div>
    )

    // return (
    //     <form onSubmit={this.onSubmit.bind(this)}>
    //         {/*<input type="text" value={this.state.value} onChange={this.onValueChange.bind(this)}/>*/}
    //         <input type="text" value={this.state.value} onChange={this.onValueChange.bind(this)}/>
    //         <input type="submit" value="Add todo"/>
    //     </form>
    // );

    // render() {
    //     return (
    //         <div>
    //             {/*<TodoForm onSubmit={this.addTodo.bind(this)}/>*/}
    //             {/*<TodoList todos={this.state.todos} onRemoveTodo={this.removeTodo.bind(this)}/>*/}
    //         </div>
    //     );
    // }
}

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
})

App = connect(mapStateToProps)(App)


