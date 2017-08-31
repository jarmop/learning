const {connect, createProvider} = ReactRedux;
const {createStore} = Redux;

let nextTodoId = 2;
const addTodo = (text) => ({
    type: 'ADD_TODO',
    id: nextTodoId++,
    text
});

const removeTodo = (id) => ({
    type: 'REMOVE_TODO',
    id
});

const myReducer = (state = [], action) => {
    switch (action.type) {
        case 'ADD_TODO':
            return {
                todos: [
                    ...state.todos,
                    {
                        id: action.id,
                        text: action.text
                    }
                ]
            };
        case 'REMOVE_TODO':
            return {
                todos: state.todos.filter(todo => todo.id !== action.id)
            };
        default:
            return state
    }
};

let App = ({dispatch, todos}) => {
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
                }}/>
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

const mapStateToProps = (state) => ({
    todos: state.todos
});

App = connect(mapStateToProps)(App);

let store = createStore(myReducer, {
    todos: [
        {
            id: 0,
            text: 'first todo'
        },
        {
            id: 1,
            text: 'second todo'
        }
    ]
});

const Provider = createProvider();

ReactDOM.render(
    <Provider store={store}>
        <App />
    </Provider>,
    document.getElementById('root')
);