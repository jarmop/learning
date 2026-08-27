package main

import "base:runtime"
import "core:fmt"
import gl "vendor:OpenGL"
import glfw "vendor:glfw"

Camera :: struct {
	pos:   [3]f32,
	front: [3]f32,
	right: [3]f32,
	up:    [3]f32,
	yaw:   f32,
	pitch: f32,
	speed: f32,
	fov:   f32,
	near:  f32,
	far:   f32,
}

camera := Camera {
	pos   = {0, 0, 4},
	front = {0.0, 0.0, -1.0},
	right = {1.0, 0.0, 0.0},
	up    = {0.0, 1.0, 0.0},
	yaw   = -90,
	pitch = -0,
	speed = 80,
	fov   = 45.0,
	near  = 0.1,
	far   = 10000.0,
}

mouse_left_pressed := false
first_cursor_pos_left := true
prev_cursor_x, prev_cursor_y: f64

io_init :: proc() {
	glfw.SetFramebufferSizeCallback(window, framebuffer_size_callback)
	glfw.SetKeyCallback(window, key_callback)
	glfw.SetMouseButtonCallback(window, mouse_button_callback)
	glfw.SetCursorPosCallback(window, cursor_pos_callback)
}

framebuffer_size_callback :: proc "c" (window: glfw.WindowHandle, width: i32, height: i32) {
	gl.Viewport(0, 0, width, height)
}

key_callback :: proc "c" (window: glfw.WindowHandle, key, scancode, action, mode: i32) {
	if key == glfw.KEY_ESCAPE && action == glfw.PRESS {
		glfw.SetWindowShouldClose(window, true)
	}
}

mouse_button_callback :: proc "c" (window: glfw.WindowHandle, button, action, mods: i32) {
	context = runtime.default_context()

	if button == glfw.MOUSE_BUTTON_LEFT {
		if action == glfw.PRESS {
			mouse_left_pressed = true
		} else {
			mouse_left_pressed = false
			first_cursor_pos_left = true
		}

		fmt.println(mouse_left_pressed)
	}
}

cursor_pos_callback :: proc "c" (window: glfw.WindowHandle, x, y: f64) {
	context = runtime.default_context()

	if mouse_left_pressed {
		if first_cursor_pos_left {
			prev_cursor_x = x
			prev_cursor_y = y
			first_cursor_pos_left = false
		}
		// world_pos.x = world_pos.x + f32(x - prev_cursor_x)
		// world_pos.y = world_pos.y + f32(y - prev_cursor_y)

		x_diff := f32(x - prev_cursor_x)
		y_diff := f32(y - prev_cursor_y)

		fmt.println("x", x_diff)
		fmt.println("y", y_diff)

		prev_cursor_x = x
		prev_cursor_y = y
	}
}
