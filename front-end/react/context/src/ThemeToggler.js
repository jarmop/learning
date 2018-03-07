import React from "react";

type Theme = 'red' | 'green';
// Pass a default theme to ensure type correctness
export const ThemeContext: Context<Theme> = React.createContext('red');

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

export default ThemeToggler;