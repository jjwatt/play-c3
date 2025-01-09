fn void main(String[] args)
{
    int! optional_value = 7;

    bool result_found = @ok(optional_value);
    assert(result_found == !@catch(optional_value));
}
