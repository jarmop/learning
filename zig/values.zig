const std = @import("std");
const print = std.debug.print;

pub fn main() void {
    const foo: u1 = 1;

    print("{}", .{foo});
}
