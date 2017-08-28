class Clock extends React.Component {
    constructor(props) {
        super(props);
        this.state = {date: new Date()};
    }

    componentDidMount() {
        this.timerID = setInterval(
            () => this.tick(),
            1000
        );

        $(document).ready(() => {
            // $('#ad').html('<script>console.log("test1");</script>');
            // $('#ad').html('<script>document.write("egerh");</script>');
        });

        // postscribe('#ad', '<script>console.log("test1");</script>');
        postscribe('#ad', '<script>document.write("egerh");</script>');
    }

    componentWillUnmount() {
        clearInterval(this.timerID);
    }

    tick() {
        this.setState({
            date: new Date()
        });
    }

    render() {
        return (
            <div>
                <h1>Hello, world!</h1>
                <h2>It is {this.state.date.toLocaleTimeString()}.</h2>
                <div id="ad">test</div>
            </div>
        );
    }
}

ReactDOM.render(
    <Clock />,
    document.getElementById('root')
);