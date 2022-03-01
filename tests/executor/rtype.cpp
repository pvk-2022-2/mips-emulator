#include "mips-emulator/executor.hpp"
#include "mips-emulator/instruction.hpp"
#include "mips-emulator/register_file.hpp"
#include "mips-emulator/register_name.hpp"

#include <catch2/catch.hpp>

using namespace mips_emulator;

using Func = Instruction::Func;

TEST_CASE("add", "[Executor]") {
    SECTION("Positive numbers") {
        using Address = typename RegisterFile::Unsigned;
        RegisterFile reg_file;

        reg_file.set_signed(RegisterName::e_t0, 1);
        reg_file.set_signed(RegisterName::e_t1, 5);

        Instruction instr(Func::e_add, RegisterName::e_t2, RegisterName::e_t0,
                          RegisterName::e_t1);

        const bool no_error = Executor::handle_rtype_instr(instr, reg_file);
        REQUIRE(no_error);

        REQUIRE(reg_file.get(RegisterName::e_t2).s == 6);
    }
}

TEST_CASE("sub", "[Executor]") {
    SECTION("Positive numbers") {
        using Address = typename RegisterFile::Unsigned;
        RegisterFile reg_file;

        reg_file.set_signed(RegisterName::e_t0, 10);
        reg_file.set_signed(RegisterName::e_t1, 1);

        Instruction instr(Func::e_sub, RegisterName::e_t2, RegisterName::e_t0,
                          RegisterName::e_t1);

        const bool no_error = Executor::handle_rtype_instr(instr, reg_file);
        REQUIRE(no_error);

        REQUIRE(reg_file.get(RegisterName::e_t2).s == 9);
    }

    SECTION("Negative numbers") {
        using Address = typename RegisterFile::Unsigned;
        RegisterFile reg_file;
        Address pc = 0;

        reg_file.set_signed(RegisterName::e_t0, -3);
        reg_file.set_signed(RegisterName::e_t1, -5);

        Instruction instr(Func::e_sub, RegisterName::e_t2, RegisterName::e_t0,
                          RegisterName::e_t1);

        const bool no_error = Executor::handle_rtype_instr(instr, reg_file);
        REQUIRE(no_error);

        REQUIRE(reg_file.get(RegisterName::e_t2).s == 2);
    }
}

TEST_CASE("or", "[Executor]") {
    SECTION("Positive numbers") {
        using Address = typename RegisterFile::Unsigned;
        RegisterFile reg_file;

        reg_file.set_unsigned(RegisterName::e_t0, 0b1);   // 1
        reg_file.set_unsigned(RegisterName::e_t1, 0b110); // 6

        Instruction instr(Func::e_or, RegisterName::e_t2, RegisterName::e_t0,
                          RegisterName::e_t1);

        const bool no_error = Executor::handle_rtype_instr(instr, reg_file);
        REQUIRE(no_error);

        REQUIRE(reg_file.get(RegisterName::e_t2).u == 0b111); // 7
    }
}

TEST_CASE("sll", "[Executor]") {
    SECTION("Shift number") {
        using Address = typename RegisterFile::Unsigned;
        RegisterFile reg_file;

        reg_file.set_unsigned(RegisterName::e_t1, 123456);

        const Instruction instr(Func::e_sll, RegisterName::e_t0,
                                RegisterName::e_0, RegisterName::e_t1, 4);

        const bool no_error = Executor::handle_rtype_instr(instr, reg_file);
        REQUIRE(no_error);

        REQUIRE(reg_file.get(RegisterName::e_t0).u == 1975296);
    }
}

// Shift instructions
TEST_CASE("sllv", "[Executor]") {
    SECTION("Shift number") {
        using Address = typename RegisterFile::Unsigned;
        RegisterFile reg_file;

        reg_file.set_unsigned(RegisterName::e_t1, 123456);
        reg_file.set_unsigned(RegisterName::e_t2, 4);

        const Instruction instr(Func::e_sllv, RegisterName::e_t0,
                                RegisterName::e_t2, RegisterName::e_t1);

        const bool no_error = Executor::handle_rtype_instr(instr, reg_file);
        REQUIRE(no_error);

        REQUIRE(reg_file.get(RegisterName::e_t0).u == 1975296);
    }

    SECTION("Correct bits used") {
        using Address = typename RegisterFile::Unsigned;
        RegisterFile reg_file;

        reg_file.set_unsigned(RegisterName::e_t1, 1);
        reg_file.set_unsigned(RegisterName::e_t2, 0xFFFFFFE1);

        const Instruction instr(Func::e_sllv, RegisterName::e_t0,
                                RegisterName::e_t2, RegisterName::e_t1);

        const bool no_error = Executor::handle_rtype_instr(instr, reg_file);
        REQUIRE(no_error);

        REQUIRE(reg_file.get(RegisterName::e_t0).u == 2);
    }
}

TEST_CASE("sra", "[Executor]") {
    SECTION("Shift positive number") {
        using Address = typename RegisterFile::Unsigned;
        RegisterFile reg_file;

        reg_file.set_unsigned(RegisterName::e_t1, 123456);

        const Instruction instr(Func::e_sra, RegisterName::e_t0,
                                RegisterName::e_0, RegisterName::e_t1, 4);

        const bool no_error = Executor::handle_rtype_instr(instr, reg_file);
        REQUIRE(no_error);

        REQUIRE(reg_file.get(RegisterName::e_t0).u == 7716);
    }

    SECTION("Shift negative number") {
        using Address = typename RegisterFile::Unsigned;
        RegisterFile reg_file;

        reg_file.set_unsigned(RegisterName::e_t1, -123456);

        const Instruction instr(Func::e_sra, RegisterName::e_t0,
                                RegisterName::e_0, RegisterName::e_t1, 4);

        const bool no_error = Executor::handle_rtype_instr(instr, reg_file);
        REQUIRE(no_error);

        REQUIRE(reg_file.get(RegisterName::e_t0).u == -7716);
    }
}

TEST_CASE("srav", "[Executor]") {
    SECTION("Shift positive number") {
        using Address = typename RegisterFile::Unsigned;
        RegisterFile reg_file;

        reg_file.set_unsigned(RegisterName::e_t1, 123456);
        reg_file.set_unsigned(RegisterName::e_t2, 4);

        const Instruction instr(Func::e_srav, RegisterName::e_t0,
                                RegisterName::e_t2, RegisterName::e_t1);

        const bool no_error = Executor::handle_rtype_instr(instr, reg_file);
        REQUIRE(no_error);

        REQUIRE(reg_file.get(RegisterName::e_t0).u == 7716);
    }

    SECTION("Shift negative number") {
        using Address = typename RegisterFile::Unsigned;
        RegisterFile reg_file;

        reg_file.set_unsigned(RegisterName::e_t1, -123456);
        reg_file.set_unsigned(RegisterName::e_t2, 4);

        const Instruction instr(Func::e_srav, RegisterName::e_t0,
                                RegisterName::e_t2, RegisterName::e_t1);

        const bool no_error = Executor::handle_rtype_instr(instr, reg_file);
        REQUIRE(no_error);

        REQUIRE(reg_file.get(RegisterName::e_t0).u == -7716);
    }

    SECTION("Correct bits used") {
        using Address = typename RegisterFile::Unsigned;
        RegisterFile reg_file;

        reg_file.set_unsigned(RegisterName::e_t1, 2);
        reg_file.set_unsigned(RegisterName::e_t2, 0xFFFFFFE1);

        const Instruction instr(Func::e_srav, RegisterName::e_t0,
                                RegisterName::e_t2, RegisterName::e_t1);

        const bool no_error = Executor::handle_rtype_instr(instr, reg_file);
        REQUIRE(no_error);

        REQUIRE(reg_file.get(RegisterName::e_t0).u == 1);
    }
}

TEST_CASE("srl", "[Executor]") {
    SECTION("Shift number") {
        using Address = typename RegisterFile::Unsigned;
        RegisterFile reg_file;

        reg_file.set_unsigned(RegisterName::e_t1, 123456);

        const Instruction instr(Func::e_srl, RegisterName::e_t0,
                                RegisterName::e_0, RegisterName::e_t1, 4);

        const bool no_error = Executor::handle_rtype_instr(instr, reg_file);
        REQUIRE(no_error);

        REQUIRE(reg_file.get(RegisterName::e_t0).u == 7716);
    }
}

TEST_CASE("srlv", "[Executor]") {
    SECTION("Shift number") {
        using Address = typename RegisterFile::Unsigned;
        RegisterFile reg_file;

        reg_file.set_unsigned(RegisterName::e_t1, 123456);
        reg_file.set_unsigned(RegisterName::e_t2, 4);

        const Instruction instr(Func::e_srlv, RegisterName::e_t0,
                                RegisterName::e_t2, RegisterName::e_t1);

        const bool no_error = Executor::handle_rtype_instr(instr, reg_file);
        REQUIRE(no_error);

        REQUIRE(reg_file.get(RegisterName::e_t0).u == 7716);
    }

    SECTION("Correct bits used") {
        using Address = typename RegisterFile::Unsigned;
        RegisterFile reg_file;

        reg_file.set_unsigned(RegisterName::e_t1, 2);
        reg_file.set_unsigned(RegisterName::e_t2, 0xFFFFFFE1);

        const Instruction instr(Func::e_srlv, RegisterName::e_t0,
                                RegisterName::e_t2, RegisterName::e_t1);

        const bool no_error = Executor::handle_rtype_instr(instr, reg_file);
        REQUIRE(no_error);

        REQUIRE(reg_file.get(RegisterName::e_t0).u == 1);
    }
}

TEST_CASE("rotr", "[Executor]") {
    const auto test_rotr = [](uint32_t amount, uint32_t input,
                              uint32_t output) {
        RegisterFile reg_file;
        reg_file.set_unsigned(RegisterName::e_t1, input);

        // ROTR is SRL with rs bit 1 = 1.
        const auto rotr_rs = static_cast<RegisterName>(1);
        const Instruction instr(Func::e_srl, RegisterName::e_t0, rotr_rs,
                                RegisterName::e_t1, amount);

        const bool no_error = Executor::handle_rtype_instr(instr, reg_file);
        REQUIRE(no_error);

        REQUIRE(reg_file.get(RegisterName::e_t0).u == output);
    };

    SECTION("Bit preservation") {
        for (int i = 0; i < 32; i++)
            test_rotr(i, ~0U, ~0U);
    }

    SECTION("Test Cases") {
        test_rotr(4, 0x162315u, 0x50016231u);
        test_rotr(2, 0x125623u, 0xc0049588u);
        test_rotr(3, 0x45324bfau, 0x48a6497fu);
        test_rotr(9, 0xdeadbeefu, 0x77ef56dfu);
        test_rotr(12, 0x124u, 0x12400000u);
        test_rotr(25, 0x4632132u, 0x31909902u);
    }
}

TEST_CASE("rotrv", "[Executor]") {
    const auto test_rotr = [](uint32_t amount, uint32_t input,
                              uint32_t output) {
        RegisterFile reg_file;

        reg_file.set_unsigned(RegisterName::e_t1, input);
        reg_file.set_unsigned(RegisterName::e_t2, amount);

        // ROTRV is SRLV with shamt bit 1 = 1.
        const Instruction instr(Func::e_srlv, RegisterName::e_t0,
                                RegisterName::e_t2, RegisterName::e_t1, 1);

        const bool no_error = Executor::handle_rtype_instr(instr, reg_file);
        REQUIRE(no_error);

        REQUIRE(reg_file.get(RegisterName::e_t0).u == output);
    };

    // Reuse tests from rotr
    SECTION("Bit preservation") {
        for (int i = 0; i < 32; i++)
            test_rotr(i, ~0U, ~0U);
    }

    SECTION("Test Cases") {
        test_rotr(4, 0x162315u, 0x50016231u);
        test_rotr(2, 0x125623u, 0xc0049588u);
        test_rotr(3, 0x45324bfau, 0x48a6497fu);
        test_rotr(9, 0xdeadbeefu, 0x77ef56dfu);
        test_rotr(12, 0x124u, 0x12400000u);
        test_rotr(25, 0x4632132u, 0x31909902u);
    }
}

// Set less than
TEST_CASE("slt", "[Executor]") {
    SECTION("Equal numbers") {
        RegisterFile reg_file;

        reg_file.set_signed(RegisterName::e_t0, 5);
        reg_file.set_signed(RegisterName::e_t1, 5);

        Instruction instr(Func::e_slt, RegisterName::e_t2, RegisterName::e_t0,
                          RegisterName::e_t1);

        const bool no_error = Executor::handle_rtype_instr(instr, reg_file);
        REQUIRE(no_error);
        REQUIRE(reg_file.get(RegisterName::e_t2).s == 0);
    }

    SECTION("Lesser number") {
        RegisterFile reg_file;

        reg_file.set_signed(RegisterName::e_t0, 2);
        reg_file.set_signed(RegisterName::e_t1, 5);

        Instruction instr(Func::e_slt, RegisterName::e_t2, RegisterName::e_t0,
                          RegisterName::e_t1);

        const bool no_error = Executor::handle_rtype_instr(instr, reg_file);
        REQUIRE(no_error);

        REQUIRE(reg_file.get(RegisterName::e_t2).s == 1);
    }
}

TEST_CASE("sltu", "[Executor]") {
    SECTION("Equal numbers") {
        RegisterFile reg_file;

        reg_file.set_unsigned(RegisterName::e_t0, 5);
        reg_file.set_unsigned(RegisterName::e_t1, 5);

        Instruction instr(Func::e_slt, RegisterName::e_t2, RegisterName::e_t0,
                          RegisterName::e_t1);

        const bool no_error = Executor::handle_rtype_instr(instr, reg_file);
        REQUIRE(no_error);

        REQUIRE(reg_file.get(RegisterName::e_t2).s == 0);
    }

    SECTION("Lesser number") {
        RegisterFile reg_file;

        reg_file.set_unsigned(RegisterName::e_t0, 2);
        reg_file.set_unsigned(RegisterName::e_t1, 5);

        Instruction instr(Func::e_slt, RegisterName::e_t2, RegisterName::e_t0,
                          RegisterName::e_t1);

        const bool no_error = Executor::handle_rtype_instr(instr, reg_file);
        REQUIRE(no_error);

        REQUIRE(reg_file.get(RegisterName::e_t2).s == 1);
    }
}

// Jump instructions
TEST_CASE("jr", "[Executor]") {

    using Unsigned = typename RegisterFile::Unsigned;
    RegisterFile reg_file;
    reg_file.set_unsigned(RegisterName::e_t0, 0xbad);

    reg_file.set_pc(0x10000000);

    Instruction instr(Func::e_jr, RegisterName::e_0, RegisterName::e_t0,
                      RegisterName::e_0);

    reg_file.update_pc(); // emulate step
    const bool no_error = Executor::handle_rtype_instr(instr, reg_file);
    REQUIRE(no_error);

    reg_file.update_pc(); // moves past delays slot

    REQUIRE(reg_file.get_pc() == 0xbad);
}

TEST_CASE("jalr", "[Executor]") {

    using Unsigned = typename RegisterFile::Unsigned;
    RegisterFile reg_file;
    reg_file.set_unsigned(RegisterName::e_t0, 0xbad);

    reg_file.set_pc(0x10beef00);

    Instruction instr(Func::e_jalr, RegisterName::e_0, RegisterName::e_t0,
                      RegisterName::e_0);

    reg_file.update_pc(); // emulate step
    const bool no_error = Executor::handle_rtype_instr(instr, reg_file);
    REQUIRE(no_error);

    reg_file.update_pc(); // moves past delays slot

    REQUIRE(reg_file.get_pc() == 0xbad);
    REQUIRE(reg_file.get(RegisterName::e_ra).u == 0x10beef04);
}

// Special operation instructions
TEST_CASE("sop30", "[Executor]") {
    SECTION("mul") {
        int32_t values[] = {-0x6FF,   0x55,        0x125,    0x7564,
                            0x523522, -0x7FCCA241, 0x23525,  0x1247,
                            0xFFFF,   INT32_MAX,   INT32_MIN};

        for (int val1 : values) {
            for (int val2 : values) {
                RegisterFile reg_file;
                reg_file.set_signed(RegisterName::e_t0, val1);
                reg_file.set_signed(RegisterName::e_t1, val2);

                Instruction instr(Func::e_sop30, RegisterName::e_t0,
                                  RegisterName::e_t0, RegisterName::e_t1, 2);

                const bool no_error =
                    Executor::handle_rtype_instr(instr, reg_file);
                REQUIRE(no_error);

                REQUIRE(reg_file.get(RegisterName::e_t0).s == val1 * val2);
            }
        }
    }

    // Using mars mult mfhi
    SECTION("muh") {
        int32_t cases[][3] = {{-0x126373, -0x126373, (int32_t)0x00000152},
                              {-0x126373, 0xF2A373, (int32_t)0xffffee92},
                              {0xABC1235, 0xF2A373, (int32_t)0x000a2ca3}};

        for (auto& test : cases) {
            const int32_t val1 = test[0];
            const int32_t val2 = test[1];
            const int32_t res = test[2];

            RegisterFile reg_file;
            reg_file.set_signed(RegisterName::e_t0, val1);
            reg_file.set_signed(RegisterName::e_t1, val2);

            Instruction instr(Func::e_sop30, RegisterName::e_t0,
                              RegisterName::e_t0, RegisterName::e_t1, 3);

            const bool no_error = Executor::handle_rtype_instr(instr, reg_file);
            REQUIRE(no_error);

            REQUIRE(reg_file.get(RegisterName::e_t0).s == res);
        }
    }
}

TEST_CASE("sop31", "[Executor]") {
    SECTION("mul") {
        uint32_t values[] = {0x6FF,    0x55,       0x125,   0x7564,
                             0x523522, 0x7FCCA241, 0x23525, 0x1247,
                             0xFFFF,   UINT32_MAX, 0};

        for (int val1 : values) {
            for (int val2 : values) {
                RegisterFile reg_file;
                reg_file.set_unsigned(RegisterName::e_t0, val1);
                reg_file.set_unsigned(RegisterName::e_t1, val2);

                Instruction instr(Func::e_sop31, RegisterName::e_t0,
                                  RegisterName::e_t0, RegisterName::e_t1, 2);

                const bool no_error =
                    Executor::handle_rtype_instr(instr, reg_file);
                REQUIRE(no_error);

                REQUIRE(reg_file.get(RegisterName::e_t0).u == val1 * val2);
            }
        }
    }

    // Using mars multu mfhi
    SECTION("muh") {
        uint32_t cases[][3] = {{0x126373, 0x126373, 0x00000152},
                               {0x126373, 0xF2A373, 0x0000116d},
                               {0xABC1235, 0xF2A373, 0x000a2ca3}};

        for (auto& test : cases) {
            const int32_t val1 = test[0];
            const int32_t val2 = test[1];
            const int32_t res = test[2];

            RegisterFile reg_file;
            reg_file.set_signed(RegisterName::e_t0, val1);
            reg_file.set_signed(RegisterName::e_t1, val2);

            Instruction instr(Func::e_sop30, RegisterName::e_t0,
                              RegisterName::e_t0, RegisterName::e_t1, 3);

            const bool no_error = Executor::handle_rtype_instr(instr, reg_file);
            REQUIRE(no_error);

            REQUIRE(reg_file.get(RegisterName::e_t0).u == res);
        }
    }
}

TEST_CASE("sel", "[Executor]") {
    SECTION("rt = 0") {
        RegisterFile reg_file;

        reg_file.set_unsigned(RegisterName::e_t0, 10);
        reg_file.set_unsigned(RegisterName::e_t1, 0);

        SECTION("eqz") {
            Instruction instr(Func::e_seleqz, RegisterName::e_t2,
                              RegisterName::e_t0, RegisterName::e_t1);
            const bool no_error = Executor::handle_rtype_instr(instr, reg_file);
            REQUIRE(no_error);

            REQUIRE(reg_file.get(RegisterName::e_t2).u == 10);
        }

        SECTION("nez") {
            Instruction instr(Func::e_selnez, RegisterName::e_t2,
                              RegisterName::e_t0, RegisterName::e_t1);
            const bool no_error = Executor::handle_rtype_instr(instr, reg_file);
            REQUIRE(no_error);

            REQUIRE(reg_file.get(RegisterName::e_t2).u == 0);
        }
    }

    SECTION("rt != 0") {
        RegisterFile reg_file;

        reg_file.set_unsigned(RegisterName::e_t0, 10);
        reg_file.set_unsigned(RegisterName::e_t1, 3);

        SECTION("eqz") {
            Instruction instr(Func::e_seleqz, RegisterName::e_t2,
                              RegisterName::e_t0, RegisterName::e_t1);
            const bool no_error = Executor::handle_rtype_instr(instr, reg_file);
            REQUIRE(no_error);

            REQUIRE(reg_file.get(RegisterName::e_t2).u == 0);
        }

        SECTION("nez") {
            Instruction instr(Func::e_selnez, RegisterName::e_t2,
                              RegisterName::e_t0, RegisterName::e_t1);
            const bool no_error = Executor::handle_rtype_instr(instr, reg_file);
            REQUIRE(no_error);

            REQUIRE(reg_file.get(RegisterName::e_t2).u == 10);
        }
    }
}
