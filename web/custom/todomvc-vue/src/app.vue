<template>
    <section class="todoapp">
        <header class="header">
            <h1>todos</h1>
            <input
                    class="new-todo"
                    placeholder="What needs to be done?"
                    autofocus
                    v-model="todoInputValue"
                    v-on:blur="addTodo"
                    v-on:keyup.13="addTodo"
            >
        </header>
        <!-- This section should be hidden by default and shown when there are todos -->
        <section class="main">
            <input id="toggle-all" class="toggle-all" type="checkbox">
            <label for="toggle-all">Mark all as complete</label>
            <ul class="todo-list">
                <li v-for="todo in todos" v-bind:class="{completed: todo.isCompleted}">
                    <div class="view">
                        <input class="toggle" type="checkbox" v-bind:checked="todo.isCompleted">
                        <label>{{ todo.text }}</label>
                        <button class="destroy"></button>
                    </div>
                    <input class="edit" value="Create a TodoMVC template">
                </li>
            </ul>
        </section>
        <!-- This footer should hidden by default and shown when there are todos -->
        <footer class="footer">
            <!-- This should be `0 items left` by default -->
            <span class="todo-count"><strong>0</strong> item left</span>
            <!-- Remove this if you don't implement routing -->
            <ul class="filters">
                <li>
                    <a class="selected" href="#/">All</a>
                </li>
                <li>
                    <a href="#/active">Active</a>
                </li>
                <li>
                    <a href="#/completed">Completed</a>
                </li>
            </ul>
            <!-- Hidden if no completed items are left ↓ -->
            <button class="clear-completed">Clear completed</button>
        </footer>
    </section>
</template>

<script>
  /**
   * Use cases:
   * - DONE add
   * - edit
   * - remove
   * - toggle completion
   * - show amount of active todos
   * - apply filters (highlight selected)
   * - clear completed
   * - save state
   */

  export default {
    data() {
      return {
        todos: [
          {
            text: 'first',
            isCompleted: true,
          },
          {
            text: 'second',
            isCompleted: false,
          }
        ],
        todoInputValue: ''
      };
    },
    methods: {
      addTodo(text) {
        if (this.todoInputValue.length === 0) {
          return;
        }
        this.todos.push({
          text: this.todoInputValue,
          isCompleted: false,
        });
        this.todoInputValue = '';
      }
    },
  };
</script>