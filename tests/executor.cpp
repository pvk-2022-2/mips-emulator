#include "mips-emulator/executor.hpp"
#include "mips-emulator/instruction.hpp"
#include "mips-emulator/register_file.hpp"
#include "mips-emulator/register_name.hpp"

#include <catch2/catch.hpp>

using namespace mips_emulator;

using Func = Instruction::Func;
using IOp = Instruction::ITypeOpcode;

TEMPLATE_TEST_CASE("add", "[Executor]", RegisterFile32, RegisterFile64) {
    SECTION("Positive numbers") {
        using Address = typename TestType::Unsigned;
        TestType reg_file;

        reg_file.set_signed(RegisterName::e_t0, 1);
        reg_file.set_signed(RegisterName::e_t1, 5);

        Instruction instr(Func::e_add, RegisterName::e_t2, RegisterName::e_t0,
                          RegisterName::e_t1);

        const bool no_error = Executor::handle_rtype_instr(instr, reg_file);
        REQUIRE(no_error);

        REQUIRE(reg_file.get(RegisterName::e_t2).s == 6);
    }
}

TEMPLATE_TEST_CASE("sub", "[Executor]", RegisterFile32, RegisterFile64) {
    SECTION("Positive numbers") {
        using Address = typename TestType::Unsigned;
        TestType reg_file;

        reg_file.set_signed(RegisterName::e_t0, 10);
        reg_file.set_signed(RegisterName::e_t1, 1);

        Instruction instr(Func::e_sub, RegisterName::e_t2, RegisterName::e_t0,
                          RegisterName::e_t1);

        const bool no_error = Executor::handle_rtype_instr(instr, reg_file);
        REQUIRE(no_error);

        REQUIRE(reg_file.get(RegisterName::e_t2).s == 9);
    }

    SECTION("Negative numbers") {
        using Address = typename TestType::Unsigned;
        TestType reg_file;
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

TEMPLATE_TEST_CASE("or", "[Executor]", RegisterFile32, RegisterFile64) {
    SECTION("Positive numbers") {
        using Address = typename TestType::Unsigned;
        TestType reg_file;

        reg_file.set_unsigned(RegisterName::e_t0, 0b1);   // 1
        reg_file.set_unsigned(RegisterName::e_t1, 0b110); // 6

        Instruction instr(Func::e_or, RegisterName::e_t2, RegisterName::e_t0,
                          RegisterName::e_t1);

        const bool no_error = Executor::handle_rtype_instr(instr, reg_file);
        REQUIRE(no_error);

        REQUIRE(reg_file.get(RegisterName::e_t2).u == 0b111); // 7
    }
}

TEMPLATE_TEST_CASE("addi", "[Executor]", RegisterFile32, RegisterFile64) {
    SECTION("Positive numbers") {
        using Address = typename TestType::Unsigned;
        TestType reg_file;

        reg_file.set_unsigned(RegisterName::e_t1, 100202);

        Instruction instr(IOp::e_addi, RegisterName::e_t0, RegisterName::e_t1,
                          22020);

        const bool no_error = Executor::handle_itype_instr(instr, reg_file);
        REQUIRE(no_error);

        REQUIRE(reg_file.get(RegisterName::e_t0).u == 122222);
    }
}

TEMPLATE_TEST_CASE("sll", "[Executor]", RegisterFile32, RegisterFile64) {
    SECTION("Shift number") {
        using Address = typename TestType::Unsigned;
        TestType reg_file;

        reg_file.set_unsigned(RegisterName::e_t1, 123456);

        const Instruction instr(Func::e_sll, RegisterName::e_t0, RegisterName::e_0, RegisterName::e_t1, 4);

        const bool no_error = Executor::handle_rtype_instr(instr, reg_file);
        REQUIRE(no_error);

        REQUIRE(reg_file.get(RegisterName::e_t0).u == 1975296);
    }
}

TEMPLATE_TEST_CASE("sllv", "[Executor]", RegisterFile32, RegisterFile64) {
    SECTION("Shift number") {
        using Address = typename TestType::Unsigned;
        TestType reg_file;

        reg_file.set_unsigned(RegisterName::e_t1, 123456);
        reg_file.set_unsigned(RegisterName::e_t2, 4);

        const Instruction instr(Func::e_sllv, RegisterName::e_t0, RegisterName::e_t2, RegisterName::e_t1);

        const bool no_error = Executor::handle_rtype_instr(instr, reg_file);
        REQUIRE(no_error);

        REQUIRE(reg_file.get(RegisterName::e_t0).u == 1975296);
    }

    SECTION("Correct bits used") {
        using Address = typename TestType::Unsigned;
        TestType reg_file;

        reg_file.set_unsigned(RegisterName::e_t1, 1);
        reg_file.set_unsigned(RegisterName::e_t2, 0xFFFFFFE1);

        const Instruction instr(Func::e_sllv, RegisterName::e_t0, RegisterName::e_t2, RegisterName::e_t1);

        const bool no_error = Executor::handle_rtype_instr(instr, reg_file);
        REQUIRE(no_error);

        REQUIRE(reg_file.get(RegisterName::e_t0).u == 2);
    }
}

TEMPLATE_TEST_CASE("sra", "[Executor]", RegisterFile32, RegisterFile64) {
    SECTION("Shift positive number") {
        using Address = typename TestType::Unsigned;
        TestType reg_file;

        reg_file.set_unsigned(RegisterName::e_t1, 123456);

        const Instruction instr(Func::e_sra, RegisterName::e_t0, RegisterName::e_0, RegisterName::e_t1, 4);

        const bool no_error = Executor::handle_rtype_instr(instr, reg_file);
        REQUIRE(no_error);

        REQUIRE(reg_file.get(RegisterName::e_t0).u == 7716);
    }

    SECTION("Shift negative number") {
        using Address = typename TestType::Unsigned;
        TestType reg_file;

        reg_file.set_unsigned(RegisterName::e_t1, -123456);

        const Instruction instr(Func::e_sra, RegisterName::e_t0, RegisterName::e_0, RegisterName::e_t1, 4);

        const bool no_error = Executor::handle_rtype_instr(instr, reg_file);
        REQUIRE(no_error);

        REQUIRE(reg_file.get(RegisterName::e_t0).u == -7716);
    }
}

TEMPLATE_TEST_CASE("srav", "[Executor]", RegisterFile32, RegisterFile64) {
    SECTION("Shift positive number") {
        using Address = typename TestType::Unsigned;
        TestType reg_file;

        reg_file.set_unsigned(RegisterName::e_t1, 123456);
        reg_file.set_unsigned(RegisterName::e_t2, 4);

        const Instruction instr(Func::e_srav, RegisterName::e_t0, RegisterName::e_t2, RegisterName::e_t1);

        const bool no_error = Executor::handle_rtype_instr(instr, reg_file);
        REQUIRE(no_error);

        REQUIRE(reg_file.get(RegisterName::e_t0).u == 7716);
    }

    SECTION("Shift negative number") {
        using Address = typename TestType::Unsigned;
        TestType reg_file;

        reg_file.set_unsigned(RegisterName::e_t1, -123456);
        reg_file.set_unsigned(RegisterName::e_t2, 4);

        const Instruction instr(Func::e_srav, RegisterName::e_t0, RegisterName::e_t2, RegisterName::e_t1);

        const bool no_error = Executor::handle_rtype_instr(instr, reg_file);
        REQUIRE(no_error);

        REQUIRE(reg_file.get(RegisterName::e_t0).u == -7716);
    }

    SECTION("Correct bits used") {
        using Address = typename TestType::Unsigned;
        TestType reg_file;

        reg_file.set_unsigned(RegisterName::e_t1, 2);
        reg_file.set_unsigned(RegisterName::e_t2, 0xFFFFFFE1);

        const Instruction instr(Func::e_srav, RegisterName::e_t0, RegisterName::e_t2, RegisterName::e_t1);

        const bool no_error = Executor::handle_rtype_instr(instr, reg_file);
        REQUIRE(no_error);

        REQUIRE(reg_file.get(RegisterName::e_t0).u == 1);
    }
}

TEMPLATE_TEST_CASE("srl", "[Executor]", RegisterFile32, RegisterFile64) {
    SECTION("Shift number") {
        using Address = typename TestType::Unsigned;
        TestType reg_file;

        reg_file.set_unsigned(RegisterName::e_t1, 123456);

        const Instruction instr(Func::e_srl, RegisterName::e_t0, RegisterName::e_0, RegisterName::e_t1, 4);

        const bool no_error = Executor::handle_rtype_instr(instr, reg_file);
        REQUIRE(no_error);

        REQUIRE(reg_file.get(RegisterName::e_t0).u == 7716);
    }
}

TEMPLATE_TEST_CASE("srlv", "[Executor]", RegisterFile32, RegisterFile64) {
    SECTION("Shift number") {
        using Address = typename TestType::Unsigned;
        TestType reg_file;

        reg_file.set_unsigned(RegisterName::e_t1, 123456);
        reg_file.set_unsigned(RegisterName::e_t2, 4);

        const Instruction instr(Func::e_srlv, RegisterName::e_t0, RegisterName::e_t2, RegisterName::e_t1);

        const bool no_error = Executor::handle_rtype_instr(instr, reg_file);
        REQUIRE(no_error);

        REQUIRE(reg_file.get(RegisterName::e_t0).u == 7716);
    }

    SECTION("Correct bits used") {
        using Address = typename TestType::Unsigned;
        TestType reg_file;

        reg_file.set_unsigned(RegisterName::e_t1, 2);
        reg_file.set_unsigned(RegisterName::e_t2, 0xFFFFFFE1);

        const Instruction instr(Func::e_srlv, RegisterName::e_t0, RegisterName::e_t2, RegisterName::e_t1);

        const bool no_error = Executor::handle_rtype_instr(instr, reg_file);
        REQUIRE(no_error);

        REQUIRE(reg_file.get(RegisterName::e_t0).u == 1);
    }
}
