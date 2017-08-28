import React from 'react';
import ReactDOM from 'react-dom';
import { App } from './App';
import { myReducer } from './App';
import { Provider } from 'react-redux'
import { createStore } from 'redux'

let store = createStore(myReducer, {todos: [
    {
        id: 0,
        text: 'first todo'
    },
    {
        id: 1,
        text: 'second todo'
    }
]});

ReactDOM.render(
    <Provider store={store}>
        <App />
    </Provider>,
    document.getElementById('root')
);
