import std::io;
import std::math;
import caca;

fn float norm(float value, float low, float high) {
    return (value - low) / (high - low);
}

fn float lerp(float low, float high, float amount) {
    return low + amount * (high - low);
}

fn float mapvalue(float value,
		  float low1, float high1,
		  float low2, float high2) {
    float n = norm(value, low1, high1);
    return lerp(low2, high2, n);
}

fn void draw_eight_eleven(caca::Canvas *cv, float power) {
    int width = caca::get_canvas_width(cv);
    int height = caca::get_canvas_height(cv);
    for (int x = 2; x < width; x += 2) {
	float n = mapvalue((float)x, 2, (float)width, -1, 1);        
	float p = math::pow(n, power);
	float ypos = lerp(10, (float)height, p);
	caca::draw_line(cv, x, 0, x, (CInt)ypos, '#');
    }
}

fn void draw_eight_eight(caca::Canvas *cv, float power) {
    int width = caca::get_canvas_width(cv);
    int height = caca::get_canvas_height(cv);
    for (int x = 0; x < width; x++) {
	// Range 0.0 to 1.0
	float n = norm((float)x, 0.0, (float)width);
	float y = math::pow(n, power);
	// Range 0.0 to 100.0
	y *= height;
	caca::put_char(cv, x, (CInt)y, '#');
    }
}

fn int main() {
    caca::Display *dp;
    caca::Canvas *cv;
    caca::Event ev;
    dp = caca::create_display(null);
    if (!dp) return 1;
    cv = caca::get_canvas(dp);
    defer {
	caca::free_canvas(cv);
	caca::free_display(dp);
    }
    int width = caca::get_canvas_width(cv);
    int height = caca::get_canvas_height(cv);
    caca::set_color_ansi(cv, caca::BLUE, caca::BLACK);
    // draw_eight_eight(cv, 0.4f);
    // draw_eight_eight(cv, 0.5f);
    // draw_eight_eight(cv, 8.0f);
    draw_eight_eight(cv, 4.0f);
    // draw_eight_eleven(cv, 4);
    // for (int x = 2; x < width; x += 2) {
    // 	float n = mapvalue((float)x, 2, (float)width, -1, 1);
    // 	float p = math::pow(n, power);
    // 	float ypos = lerp(10, (float)height, p);
    // 	caca::draw_line(cv, x, 0, x, (CInt)ypos, '#');
    // 	char[50] buffer;
    // 	String fstr = "n: %s";
    // 	ZString res = (ZString)io::bprintf(&buffer, fstr, n)!!;
    // 	caca::put_str(cv, width - 30, height - 2, (CChar*)res);
    // }
    caca::refresh_display(dp);
    caca::get_event(dp, caca::EVENT_KEY_PRESS, ev, -1);
    io::printfn("width: %d", width);
    io::printfn("height: %d", height);
    return 0;
}
