import React, {Component} from 'react';
import logo from './logo.svg';
import './App.css';

class App extends Component {
    render() {
        return (
            <div className="App">
                <header className="App-header">
                    <img src={logo} className="App-logo" alt="logo"/>
                    <h1 className="App-title">Welcome to React</h1>
                </header>
                <p className="App-intro">
                    To get started, edit <code>src/App.js</code> and save to reload.
                </p>
                <ThemeToggler>
                    <Title>
                        title
                    </Title>
                </ThemeToggler>
            </div>
        );
    }
}

export default App;

type Theme = 'red' | 'green';
// Pass a default theme to ensure type correctness
const ThemeContext: Context<Theme> = React.createContext('red');

class ThemeToggler extends React.Component {
    state = {theme: 'red'};

    render() {
        return (
            // Pass the current context value to the Provider's `value` prop.
            // Changes are detected using strict comparison (Object.is)
            <ThemeContext.Provider value={this.state.theme}>
                <button
                    onClick={() =>
                        this.setState(state => ({
                            theme: state.theme === 'red' ? 'green' : 'red',
                        }))
                    }>
                    Toggle theme
                </button>
                {this.props.children}
            </ThemeContext.Provider>
        );
    }
}

class Title extends React.Component {
    render() {
        return (
            // The Consumer uses a render prop API. Avoids conflicts in the
            // props namespace.
            <ThemeContext.Consumer>
                {theme => (
                    <h1 style={{color: theme === 'red' ? '#f00' : '#0f0'}}>
                        {this.props.children}
                    </h1>
                )}
            </ThemeContext.Consumer>
        );
    }
}
