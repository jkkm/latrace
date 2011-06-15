. $PWD/test/script/functions.sh

function tls_cleanup
{
        rm -f /tmp/tls_latrace.conf
}

function test_libsym_all
{
	LD_LIBRARY_PATH=$PWD ./latrace -qR $PWD/test-libsym all > /dev/null
	CHECK_RET
}

function test_libsym_lib1
{
	cat > /tmp/tls_latrace.conf <<EOF
OPTIONS {
        LIBS=*lib-test-libsym1
}
EOF
	LD_LIBRARY_PATH=$PWD ./latrace -qR -N /tmp/tls_latrace.conf $PWD/test-libsym lib1 > /dev/null
	CHECK_RET
}

function test_libsym_lib2
{
	cat > /tmp/tls_latrace.conf <<EOF
OPTIONS {
        LIBS=*lib-test-libsym2
}
EOF
	LD_LIBRARY_PATH=$PWD ./latrace -qR -N /tmp/tls_latrace.conf $PWD/test-libsym lib2 > /dev/null
	CHECK_RET
}

function test_libsym_lib3
{
	cat > /tmp/tls_latrace.conf <<EOF
OPTIONS {
        LIBS=*lib-test-libsym3
}
EOF
	LD_LIBRARY_PATH=$PWD ./latrace -qR -N /tmp/tls_latrace.conf $PWD/test-libsym lib3 > /dev/null
	CHECK_RET
}

function test_libsym_lib1lib2
{
	cat > /tmp/tls_latrace.conf <<EOF
OPTIONS {
        LIBS=*lib-test-libsym1,*lib-test-libsym2
}
EOF
	LD_LIBRARY_PATH=$PWD ./latrace -qR -N /tmp/tls_latrace.conf $PWD/test-libsym lib1lib2 > /dev/null
	CHECK_RET
}

function test_libsym_lib2lib3
{
	cat > /tmp/tls_latrace.conf <<EOF
OPTIONS {
        LIBS=*lib-test-libsym2,*lib-test-libsym3
}
EOF
	LD_LIBRARY_PATH=$PWD ./latrace -qR -N /tmp/tls_latrace.conf $PWD/test-libsym lib2lib3 > /dev/null
	CHECK_RET
}

function test_libsym_lib1lib2lib3
{
	cat > /tmp/tls_latrace.conf <<EOF
OPTIONS {
        LIBS=*lib-test-libsym1,*lib-test-libsym2,*lib-test-libsym3
}
EOF
	LD_LIBRARY_PATH=$PWD ./latrace -qR -N /tmp/tls_latrace.conf $PWD/test-libsym lib1lib2lib3 > /dev/null
	CHECK_RET
}

function test_libsym_sym1
{
	cat > /tmp/tls_latrace.conf <<EOF
OPTIONS {
        SYM=test_libsym1_1
}
EOF
	LD_LIBRARY_PATH=$PWD ./latrace -qR -N /tmp/tls_latrace.conf $PWD/test-libsym sym1 > /dev/null
	CHECK_RET
}

function test_libsym_sym2sym3
{
	cat > /tmp/tls_latrace.conf <<EOF
OPTIONS {
        SYM=test_libsym2_2,test_libsym3_3
}
EOF
	LD_LIBRARY_PATH=$PWD ./latrace -qR -N /tmp/tls_latrace.conf $PWD/test-libsym sym2sym3 > /dev/null
	CHECK_RET
}

function test_libsym_omit_sym1
{
	cat > /tmp/tls_latrace.conf <<EOF
OPTIONS {
        SYM_OMIT=test_libsym1_1
}
EOF
	LD_LIBRARY_PATH=$PWD ./latrace -qR -N /tmp/tls_latrace.conf $PWD/test-libsym omit_sym1 > /dev/null
	CHECK_RET
}

function test_libsym_omit_sym2sym3
{
	cat > /tmp/tls_latrace.conf <<EOF
OPTIONS {
        SYM_OMIT=test_libsym2_3,test_libsym3_2
}
EOF
	LD_LIBRARY_PATH=$PWD ./latrace -qR -N /tmp/tls_latrace.conf $PWD/test-libsym omit_sym2sym3 > /dev/null
	CHECK_RET
}

function test_libsym
{
	test_libsym_all
	test_libsym_lib1
	test_libsym_lib2
	test_libsym_lib3
	test_libsym_lib1lib2
	test_libsym_lib2lib3
	test_libsym_lib1lib2lib3
	test_libsym_sym1
	test_libsym_sym2sym3
	test_libsym_omit_sym1
	test_libsym_omit_sym2sym3

# TODO tests for LIBS_TO, LIBS_FROM, SYM_BELOW, SYM_NOEXIT

	tls_cleanup
}
