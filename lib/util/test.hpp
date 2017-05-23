#pragma once

auto test() -> const Buffer {
    Buffer buffer(200, 100);
    buffer.map([&](const Color & c, u32 x, u32 y, const Buffer & b) -> Color {
        return Color(
            f32(x) / f32(buffer.width),
            f32(y) / f32(buffer.height),
            f32(0.2)
        );
    });
    return buffer;
}