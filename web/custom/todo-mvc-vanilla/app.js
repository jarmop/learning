/**
 * Use cases:
 * - DONE add
 * - DONE remove
 * - DONE toggle completion
 * - DONE apply filters
 * - DONE clear completed
 * - DONE show amount of active todos
 * - DONE save todo texts to local storage
 * - DONE highlight selected filter
 * - DONE save todo completion status to local storage
 * - DONE edit todos
 */

const KEY_ENTER = 'Enter';

let todoList = document.querySelector('.todo-list');

const closeEdit = input => {
  let listItem = input.parentElement;
  listItem.querySelector('label').innerHTML = input.value;
  listItem.className = listItem.className.replace('editing', '');
  updateState();
};

const updateActiveCount = () => {
  let activeCount = document.querySelectorAll('.todo-list li:not(.completed)')
    .length;
  let itemOrItems = activeCount > 1 ? 'items' : 'item';
  document.querySelector(
    '.todo-count'
  ).innerHTML = `<strong>${activeCount}</strong> ${itemOrItems} left`;
};

const updateState = () => {
  updateActiveCount();
  let todos = [];
  document.querySelectorAll('.todo-list li').forEach(listItem =>
    todos.push({
      text: listItem.querySelector('label').innerHTML,
      completed: listItem.className === 'completed',
    })
  );
  localStorage.setItem('todos', JSON.stringify(todos));
};

const clearCompleted = () => {
  document
    .querySelectorAll('.completed')
    .forEach(listItem => listItem.remove());
  updateState();
};

const applyFilter = filter => {
  let handleVisibility = listItem => {
    listItem.style = 'display: list-item';
  };
  if (filter === 'completed') {
    handleVisibility = listItem => {
      if (listItem.className === filter) {
        listItem.style = 'display: list-item';
      } else {
        listItem.style = 'display: none';
      }
    };
  } else if (filter === 'active') {
    handleVisibility = listItem => {
      if (!listItem.className) {
        listItem.style = 'display: list-item';
      } else {
        listItem.style = 'display: none';
      }
    };
  }
  document.querySelectorAll('.todo-list li').forEach(handleVisibility);
};

const parseFilterFromUrl = url => {
  return url.substring(url.lastIndexOf('/') + 1);
};

const setFilter = filter => {
  applyFilter(filter);
  document.querySelectorAll('.filters li a').forEach(filterLink => {
    filterLink.className =
      parseFilterFromUrl(filterLink.href) === filter ? 'selected' : '';
  });
};

const bindToggle = toggleButton => {
  toggleButton.onclick = event => {
    let listItem = event.target.parentElement.parentElement;
    if (listItem.className) {
      listItem.className = '';
    } else {
      listItem.className = 'completed';
    }
    updateState();
  };
};

const bindDestruction = destroyButton => {
  destroyButton.onclick = event => {
    event.target.parentElement.parentElement.remove();
    updateState();
  };
};

const bindEdit = label => {
  label.ondblclick = event => {
    let listItem = event.target.parentElement.parentElement;
    listItem.className += ' editing';
    let input = document.createElement('input');
    input.value = label.innerHTML;
    input.className = 'edit';
    input.onclick = event => event.stopPropagation();
    listItem.appendChild(input);
    input.focus();
  };
};

const addTodo = text => {
  if (!text) {
    return null;
  }
  let toggleButton = document.createElement('input');
  toggleButton.className = 'toggle';
  toggleButton.setAttribute('type', 'checkbox');
  bindToggle(toggleButton);

  let label = document.createElement('label');
  label.className = 'todo-label';
  label.innerHTML = text;
  bindEdit(label);

  let destroyButton = document.createElement('button');
  destroyButton.className = 'destroy';
  bindDestruction(destroyButton);

  let view = document.createElement('div');
  view.className = 'view';
  view.appendChild(toggleButton);
  view.appendChild(label);
  view.appendChild(destroyButton);

  let listItem = document.createElement('li');
  listItem.appendChild(view);

  todoList.appendChild(listItem);

  updateState();

  return listItem;
};

window.onkeypress = event => {
  if (event.key === KEY_ENTER) {
    let input = event.target;
    if (input.className === 'new-todo') {
      addTodo(event.target.value);
      input.value = '';
    } else if (input.className === 'edit') {
      closeEdit(input);
    }
  }
};

document.querySelectorAll('.filters li a').forEach(filterLink => {
  filterLink.onclick = event => {
    setFilter(parseFilterFromUrl(event.target.href));
  };
});

document.querySelector('.clear-completed').onclick = () => {
  clearCompleted();
};

JSON.parse(localStorage.getItem('todos')).map(todo => {
  todoElement = addTodo(todo.text);
  if (todo.completed) {
    todoElement.className = 'completed';
    todoElement.querySelector('.toggle').checked = true;
  }
});

setFilter(parseFilterFromUrl(window.location.hash));
updateState();

window.onclick = event => {
  document.querySelectorAll('.edit').forEach(todoInput => {
    closeEdit(todoInput);
  });
};
