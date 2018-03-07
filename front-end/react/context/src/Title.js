import React from "react";
import {ThemeContext} from "./ThemeToggler";

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

export default Title;