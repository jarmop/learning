import React from "react";
import {ThemeContext} from "./ThemeToggler";

const Title = ({children}) => (
    // The Consumer uses a render prop API. Avoids conflicts in the
    // props namespace.
    <ThemeContext.Consumer>
        {({theme}) => {
            return (
                <h1 style={{color: theme.color === 'red' ? '#f00' : '#0f0'}}>
                    {children}
                    <Postfix/>
                </h1>
            );
        }}
    </ThemeContext.Consumer>
);

export default Title;

const Postfix = () => (
    <ThemeContext.Consumer>
        {({theme}) => (
            <span>-{theme.name}</span>
        )}
    </ThemeContext.Consumer>
);