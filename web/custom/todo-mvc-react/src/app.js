/**
 * Use cases:
 * - DONE add
 * - DONE edit todos
 * - remove
 * - toggle completion
 * - show amount of active todos
 * - apply filters (highlight selected)
 * - clear completed
 * - save state
 */

import React from 'react';

const KEY_ENTER = 'Enter';

class App extends React.Component {
  constructor(props) {
    super(props);

    this.state = {
      todos: [
        {
          text: 'dummy',
          completed: true,
        },
      ],
    };

    this.addTodo = this.addTodo.bind(this);
    this.changeTodo = this.changeTodo.bind(this);
  }

  addTodo(value) {
    if (value.length === 0) {
      return;
    }

    this.setState({
      todos: [
        ...this.state.todos,
        {
          text: value,
          completed: false,
        },
      ],
    });
  }

  changeTodo(changedIndex, changedTodo) {
    this.setState({
      todos: this.state.todos.map(
          (todo, index) => index === changedIndex ? changedTodo : todo,
      ),
    });
  }

  render() {
    return (
        <section className="todoapp">
          <header className="header">
            <h1>todos</h1>
            <TodoInput onAdd={this.addTodo}/>
          </header>
          <section className="main">
            <input id="toggle-all" className="toggle-all" type="checkbox"/>
            <label htmlFor="toggle-all">Mark all as complete</label>
            <ul className="todo-list">
              {this.state.todos.map((todo, index) =>
                  <Todo
                      key={index}
                      todo={todo}
                      onChange={todo => this.changeTodo(index, todo)}
                  />,
              )}
            </ul>
          </section>
          <footer className="footer">
            <span className="todo-count"><strong>0</strong> item left</span>
            <ul className="filters">
              <li>
                <a className="selected" href="#/">All</a>
              </li>
              <li>
                <a href="#/active">Active</a>
              </li>
              <li>
                <a href="#/completed">Completed</a>
              </li>
            </ul>
            <button className="clear-completed">Clear completed</button>
          </footer>
        </section>
    );

  }
}

class Todo extends React.Component {
  constructor(props) {
    super(props);

    this.state = {
      editing: false,
    };
  }

  save(value) {
    this.setState({editing: false});
    this.props.onChange({
      ...this.props.todo,
      text: value,
    });
  }

  render() {
    let {todo, onChange} = this.props;

    let classes = [];
    if (todo.completed) {
      classes.push('completed');
    }
    if (this.state.editing) {
      classes.push('editing');
    }

    return (
        <li className={classes.join(' ')}>
          {this.state.editing
              ?
              <input
                  className="edit"
                  defaultValue={todo.text}
                  autoFocus={true}
                  onBlur={event => this.save(event.target.value)}
                  onKeyPress={event => event.key === KEY_ENTER &&
                      this.save(event.target.value)}
              />
              :
              <div className="view">
                <input
                    className="toggle"
                    type="checkbox"
                    defaultChecked={todo.completed}
                />
                <label onDoubleClick={event => this.setState({editing: true})}>
                  {todo.text}
                </label>
                <button className="destroy"></button>
              </div>
          }
        </li>
    );
  }
}

const TodoInput = ({onAdd}) => (
    <input
        className="new-todo"
        placeholder="What needs to be done?"
        autoFocus
        onBlur={event => {
          onAdd(event.target.value);
          event.target.value = '';
        }}
        onKeyDown={event => {
          if (event.key === KEY_ENTER) {
            onAdd(event.target.value);
            event.target.value = '';
          }
        }}
    />
);

export default App;