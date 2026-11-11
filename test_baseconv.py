import subprocess
import unittest


class BaseconvTests(unittest.TestCase):
    # Case Tuple index constants
    ARG = 0
    OUT = 1
    ERR = 2

    executable = ["./build/baseconv"]
    whitespace_size = 12
    whitespace = " " * whitespace_size
    col_per_row = 5
    header_row = (
        f"Decimal{whitespace}Hexadecimal{whitespace}Octal{whitespace}ASCII"
        f"{whitespace}Binary\n"
    )
    formatting_err_msg = (
        "Usage: baseconv <val1> [val2 ...]\n"
        'Formats: 123 (decimal), 0x123 (hex), 0o123 (oct), "a" (ASCII), b1001 (binary)\n'
    )
    input_out_of_bounds_err = "Inputs must be between 2^63 - 1 and -2^63\n"
    input_too_long_err = "Inputs must not exceed 99 characters\n"

    def run_baseconv(self, args: list):
        cmd = self.executable + args
        return subprocess.run(cmd, capture_output=True, text=True, check=False)

    def assert_output(self, result, lo_outputs: list[str], err: str):
        output_rows = ""
        if lo_outputs:
            output_rows = self.header_row
        for row_num in range(len(lo_outputs) // self.col_per_row):
            output_rows += (
                f"{lo_outputs[row_num * self.col_per_row + 0]:<19}"
                f"{lo_outputs[row_num * self.col_per_row + 1]:<23}"
                f"{lo_outputs[row_num * self.col_per_row + 2]:<17}"
                f"{lo_outputs[row_num * self.col_per_row + 3]:<17}"
                f"{lo_outputs[row_num * self.col_per_row + 4]}"
                "\n"
            )
        expected_exit_code = 1 if err else 0
        with self.subTest(check="exit code"):
            self.assertEqual(
                result.returncode,
                expected_exit_code,
            )

        with self.subTest(check="stdout"):
            self.assertEqual(result.stdout, output_rows)

        with self.subTest(check="stderr"):
            self.assertEqual(result.stderr, err)

    def check_cases(self, cases: list[tuple]):
        for index in range(len(cases)):
            with self.subTest(arg=cases[index][self.ARG]):
                result = self.run_baseconv(cases[index][self.ARG])
                self.assert_output(result, cases[index][self.OUT], cases[index][self.ERR])

    def test_formatting(self):
        cases = [
            (
                ["6928"],
                ["6928", "0x1b10", "0o15420", "Inval", "1101100010000"],
                "",
            ),
            (["62.32"], [], self.formatting_err_msg),
            (["f9203"], [], self.formatting_err_msg),
            (
                ["0XFFF"],
                ["4095", "0xfff", "0o7777", "Inval", "111111111111"],
                "",
            ),
            (["FC3"], [], self.formatting_err_msg),
            (["0XFG"], [], self.formatting_err_msg),
            (["0O357"], ["239", "0xef", "0o357", "NonPr", "11101111"], ""),
            (["o356"], [], self.formatting_err_msg),
            (["0O368"], [], self.formatting_err_msg),
            (["'A'"], ["65", "0x41", "0o101", "A", "1000001"], ""),
            (['"|"'], ["124", "0x7c", "0o174", "|", "1111100"], ""),
            (['"p"'], ["112", "0x70", "0o160", "p", "1110000"], ""),
            (["A"], [], self.formatting_err_msg),
            (["'ca'"], [], self.formatting_err_msg),
            (["'A\""], [], self.formatting_err_msg),
            (["B100101"], ["37", "0x25", "0o45", "%", "100101"], ""),
            (["b0102"], [], self.formatting_err_msg),
            (["bi0101"], [], self.formatting_err_msg),
        ]
        self.check_cases(cases)

    def test_leading_zeros(self):
        cases = [
            (["0"], ["0", "0x0", "0o0", "NonPr", "0"], ""),
            (["013"], [], self.formatting_err_msg),
            (["0x00f"], ["15", "0xf", "0o17", "NonPr", "1111"], ""),
            (["0O007"], ["7", "0x7", "0o7", "NonPr", "111"], ""),
            (["B001"], ["1", "0x1", "0o1", "NonPr", "1"], ""),
        ]
        self.check_cases(cases)

    def test_length_restriction(self):
        cases = [
            (["0x" + "0" * 98 + "1"], ["1", "0x1", "0o1", "NonPr", "1"], ""),
            (["0x" + "0" * 99 + "2"], [], self.input_too_long_err),
        ]
        self.check_cases(cases)

    def test_negative_inputs(self):
        cases = [
            (
                ["-23"],
                [
                    "-23",
                    "0xffffffe9",
                    "0o37777777751",
                    "Inval",
                    "11111111111111111111111111101001",
                ],
                "",
            ),
            (
                ["-0XFF"],
                [
                    "-255",
                    "0xffffff01",
                    "0o37777777401",
                    "Inval",
                    "11111111111111111111111100000001",
                ],
                "",
            ),
            (
                ["-0O77"],
                [
                    "-63",
                    "0xffffffc1",
                    "0o37777777701",
                    "Inval",
                    "11111111111111111111111111000001",
                ],
                "",
            ),
            (["-'A'"], [], self.formatting_err_msg),
            (
                ["-B101"],
                [
                    "-5",
                    "0xfffffffb",
                    "0o37777777773",
                    "Inval",
                    "11111111111111111111111111111011",
                ],
                "",
            ),
        ]
        self.check_cases(cases)

    def test_overflow_wrappin(self):
        cases = [
            (
                ["2147483647"],
                [
                    "2147483647",
                    "0x7fffffff",
                    "0o17777777777",
                    "Inval",
                    "1111111111111111111111111111111",
                ],
                "",
            ),
            (
                ["2147483648"],
                [
                    "-2147483648",
                    "0x80000000",
                    "0o20000000000",
                    "Inval",
                    "10000000000000000000000000000000",
                ],
                "",
            ),
            (
                ["-2147483648"],
                [
                    "-2147483648",
                    "0x80000000",
                    "0o20000000000",
                    "Inval",
                    "10000000000000000000000000000000",
                ],
                "",
            ),
            (
                ["-2147483649"],
                [
                    "2147483647",
                    "0x7fffffff",
                    "0o17777777777",
                    "Inval",
                    "1111111111111111111111111111111",
                ],
                "",
            ),
            (
                ["0xffffffff1"],
                [
                    "-15",
                    "0xfffffff1",
                    "0o37777777761",
                    "Inval",
                    "11111111111111111111111111110001",
                ],
                "",
            ),
            (
                ["0o377777777771"],
                [
                    "-7",
                    "0xfffffff9",
                    "0o37777777771",
                    "Inval",
                    "11111111111111111111111111111001",
                ],
                "",
            ),
            (
                ["B11111111111111111111111111111111"],
                [
                    "-1",
                    "0xffffffff",
                    "0o37777777777",
                    "Inval",
                    "11111111111111111111111111111111",
                ],
                "",
            ),
            (
                ["-6442450945"],
                [
                    "2147483647",
                    "0x7fffffff",
                    "0o17777777777",
                    "Inval",
                    "1111111111111111111111111111111",
                ],
                "",
            ),
            (
                ["10737418240"],
                [
                    "-2147483648",
                    "0x80000000",
                    "0o20000000000",
                    "Inval",
                    "10000000000000000000000000000000",
                ],
                "",
            ),
            (
                ["0x7FFFFFFFFFFFFFFF"],
                [
                    "-1",
                    "0xffffffff",
                    "0o37777777777",
                    "Inval",
                    "11111111111111111111111111111111",
                ],
                "",
            ),
            (["0x8000000000000000"], [], self.input_out_of_bounds_err),
            (["-0x8000000000000000"], ["0", "0x0", "0o0", "NonPr", "0"], ""),
            (["-0x8000000000000001"], [], self.input_out_of_bounds_err),
        ]
        self.check_cases(cases)

    def test_multiple_inputs(self):
        cases = [
            (
                [],
                [],
                self.formatting_err_msg,
            ),
            (
                ["23", "B1001"],
                [
                    "23",
                    "0x17",
                    "0o27",
                    "NonPr",
                    "10111",
                    "9",
                    "0x9",
                    "0o11",
                    "NonPr",
                    "1001",
                ],
                "",
            ),
            (
                ["'cat'", "010"],
                [],
                self.formatting_err_msg,
            ),
            (
                ["10", "'cat'", "0XFF"],
                ["10", "0xa", "0o12", "NonPr", "1010"],
                self.formatting_err_msg,
            ),
            (
                ["10", "0XFF", "B101", "'cat'", "0O77"],
                [
                    "10",
                    "0xa",
                    "0o12",
                    "NonPr",
                    "1010",
                    "255",
                    "0xff",
                    "0o377",
                    "NonPr",
                    "11111111",
                    "5",
                    "0x5",
                    "0o5",
                    "NonPr",
                    "101",
                ],
                self.formatting_err_msg,
            ),
            (
                ["'cat'", "10", "0XFF", "B101", "0O77", "O32", "978"],
                [],
                self.formatting_err_msg,
            ),
        ]
        self.check_cases(cases)


if __name__ == "__main__":
    unittest.main()
