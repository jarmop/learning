import React from "react";

// Pass a default theme to ensure type correctness
export const ThemeContext = React.createContext();

class ThemeToggler extends React.Component {
    state = {
        theme: {
            color: 'red',
            name: 'ruby',
        }
    };

    render() {
        return (
            // Pass the current context value to the Provider's `value` prop.
            // Changes are detected using strict comparison (Object.is)
            <ThemeContext.Provider value={this.state.theme}>
                <button
                    onClick={() =>
                        this.setState(state => ({
                            theme: {
                                color: state.theme.color === 'red' ? 'green' : 'red',
                                name: state.theme.name === 'ruby' ? 'emerald' : 'ruby',
                            }
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