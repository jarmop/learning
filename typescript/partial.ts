export default function () {
    interface Todo {
        title: string;
        description: string;
    }

    function updateTodo(todo: Todo, fieldsToUpdate: Partial<Todo>) {
        return { ...todo, ...fieldsToUpdate };
    }

    const todo = {
        title: 'wrth',
        description: 'ukeey',
    }

    updateTodo(todo, { description: 'khdgh' })

    // updateTodo(todo, { foo: 'bar' }) // error
}