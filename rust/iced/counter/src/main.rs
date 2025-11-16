use iced::widget;

struct Counter {
    // This will be our state of the counter app
    // a.k.a the current count value
    count: i32,
}

#[derive(Debug, Clone, Copy)]
enum Message {
    
    // Emitted when decrement ("-") button is pressed
    DecrementCount,
    // Emitted when the increment ("+") button is pressed
    IncrementCount,
}

// Implement our Counter
impl Counter {
    fn new() -> Self {
        // initialize the counter struct
        // with count value as 0.
        Self { count: 0 }
    }

    fn update(&mut self, message: Message) -> iced::Task<Message> {
        // handle emitted messages
        match message {
            Message::IncrementCount => self.count += 1,
            Message::DecrementCount => self.count -= 1,
        }
        iced::Task::none()
    }

    fn view(&self) -> iced::Element<'_, Message> {
        // create the View Logic (UI)
        let row = widget::row![
            widget::button("-").on_press(Message::DecrementCount),
            widget::text!("Count: {}", self.count),
            widget::button("+").on_press(Message::IncrementCount),
        ]
        .spacing(10);
        widget::container(row)
            .center_x(iced::Length::Fill)
            .center_y(iced::Length::Fill)
            .width(iced::Length::Fill)
            .height(iced::Length::Fill)
            .into()
    }
}

fn main() -> iced::Result {
    // run the app from main function
    iced::application("Counter Example", Counter::update, Counter::view)
        .run_with(|| (Counter::new(), iced::Task::none()))
}