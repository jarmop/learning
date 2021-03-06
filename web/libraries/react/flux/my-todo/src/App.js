import React, {Component} from 'react';
import './App.css';

class App extends Component {
    constructor(props) {
        super(props)
        this.state = {
            todos: [
                'first todo',
                'second todo',
            ]
        }
    }

    addTodo(text) {
        let todos = this.state.todos;
        todos.push(text);
        this.setState(todos);
    }

    removeTodo(indexToRemove) {
        let todos = this.state.todos;
        todos = todos.filter((todo, index) => index !== indexToRemove);
        this.setState({'todos': todos});
    }

    render() {
        return (
            <div>
                <TodoForm onSubmit={this.addTodo.bind(this)}/>
                <TodoList todos={this.state.todos} onRemoveTodo={this.removeTodo.bind(this)}/>
            </div>
        );
    }
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



export default App;
