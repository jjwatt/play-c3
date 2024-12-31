import caca;

fn void main()
{
    caca::Display *dp;
    caca::Canvas *cv;
    dp = caca::create_display(null);
    cv = caca::get_canvas(dp);

    caca::set_color_ansi(cv, caca::BLUE, caca::BLACK);
    caca::fill_box(cv, 10, 10, 10, 10, '#');
    caca::refresh_display(dp);

    caca::get_event(dp, caca::EVENT_KEY_PRESS, null, -1);
    caca::free_canvas(cv);
    caca::free_display(dp);
}
