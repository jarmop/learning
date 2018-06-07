/**
 * Use cases:
 * - DONE add
 * - DONE edit todos
 * - DONE remove
 * - DONE toggle completion
 * - DONE show amount of active todos
 * - apply filters (highlight selected)
 * - clear completed
 * - save state
 * - use Flow?
 * - use Redux?
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
    this.removeTodo = this.removeTodo.bind(this);
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

  removeTodo(indexToRemove) {
    this.setState({
      todos: this.state.todos.filter((todo, index) => index !== indexToRemove)
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
                      onRemove={() => this.removeTodo(index)}
                  />,
              )}
            </ul>
          </section>
          <footer className="footer">
            <TodoCount count={this.state.todos.length}/>
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

  save(todo) {
    this.setState({editing: false});
    this.props.onChange(todo);
  }

  saveText(value) {
    this.save({
      ...this.props.todo,
      text: value,
    });
  }

  saveCompletion(completion) {
    this.save({
      ...this.props.todo,
      completed: completion,
    });
  }

  render() {
    let {todo, onRemove} = this.props;

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
                  onBlur={event => this.saveText(event.target.value)}
                  onKeyPress={event => event.key === KEY_ENTER &&
                      this.saveText(event.target.value)}
              />
              :
              <div className="view">
                <input
                    className="toggle"
                    type="checkbox"
                    defaultChecked={todo.completed}
                    onChange={event => this.saveCompletion(event.target.checked)}
                />
                <label onDoubleClick={event => this.setState({editing: true})}>
                  {todo.text}
                </label>
                <button className="destroy" onClick={event => onRemove()}></button>
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

const TodoCount = ({count}) => (
    <span className="todo-count"><strong>{count}</strong> item{count === 1 ? '' : 's'} left</span>
);

export default App;