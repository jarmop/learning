function greeter(person: string) {
    return 'Hello, ' + person;
}

export default () => {
    let user = "Jane User";

    console.log(greeter(user));
}
