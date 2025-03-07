﻿
### --- BORINGSSL ---###

# TODO: add skia-opengl-emscripten/src/chromium/third_party/boringssl/src/ssl/CMakeLists.txt

set(BORINGSSL_SOURCES
  ${BORINGSSL_DIR}err_data.c
  ${BORINGSSL_DIR}src/crypto/asn1/a_bitstr.c
  ${BORINGSSL_DIR}src/crypto/asn1/a_bool.c
  ${BORINGSSL_DIR}src/crypto/asn1/a_d2i_fp.c
  ${BORINGSSL_DIR}src/crypto/asn1/a_dup.c
  ${BORINGSSL_DIR}src/crypto/asn1/a_enum.c
  ${BORINGSSL_DIR}src/crypto/asn1/a_gentm.c
  ${BORINGSSL_DIR}src/crypto/asn1/a_i2d_fp.c
  ${BORINGSSL_DIR}src/crypto/asn1/a_int.c
  ${BORINGSSL_DIR}src/crypto/asn1/a_mbstr.c
  ${BORINGSSL_DIR}src/crypto/asn1/a_object.c
  ${BORINGSSL_DIR}src/crypto/asn1/a_octet.c
  ${BORINGSSL_DIR}src/crypto/asn1/a_print.c
  ${BORINGSSL_DIR}src/crypto/asn1/a_strnid.c
  ${BORINGSSL_DIR}src/crypto/asn1/a_time.c
  ${BORINGSSL_DIR}src/crypto/asn1/a_type.c
  ${BORINGSSL_DIR}src/crypto/asn1/a_utctm.c
  ${BORINGSSL_DIR}src/crypto/asn1/a_utf8.c
  ${BORINGSSL_DIR}src/crypto/asn1/asn1_lib.c
  #${BORINGSSL_DIR}src/crypto/asn1/asn1_locl.h
  ${BORINGSSL_DIR}src/crypto/asn1/asn1_par.c
  ${BORINGSSL_DIR}src/crypto/asn1/asn_pack.c
  ${BORINGSSL_DIR}src/crypto/asn1/f_enum.c
  ${BORINGSSL_DIR}src/crypto/asn1/f_int.c
  ${BORINGSSL_DIR}src/crypto/asn1/f_string.c
  ${BORINGSSL_DIR}src/crypto/asn1/tasn_dec.c
  ${BORINGSSL_DIR}src/crypto/asn1/tasn_enc.c
  ${BORINGSSL_DIR}src/crypto/asn1/tasn_fre.c
  ${BORINGSSL_DIR}src/crypto/asn1/tasn_new.c
  ${BORINGSSL_DIR}src/crypto/asn1/tasn_typ.c
  ${BORINGSSL_DIR}src/crypto/asn1/tasn_utl.c
  ${BORINGSSL_DIR}src/crypto/asn1/time_support.c
  ${BORINGSSL_DIR}src/crypto/base64/base64.c
  ${BORINGSSL_DIR}src/crypto/bio/bio.c
  ${BORINGSSL_DIR}src/crypto/bio/bio_mem.c
  ${BORINGSSL_DIR}src/crypto/bio/connect.c
  ${BORINGSSL_DIR}src/crypto/bio/fd.c
  ${BORINGSSL_DIR}src/crypto/bio/file.c
  ${BORINGSSL_DIR}src/crypto/bio/hexdump.c
  #${BORINGSSL_DIR}src/crypto/bio/internal.h
  ${BORINGSSL_DIR}src/crypto/bio/pair.c
  ${BORINGSSL_DIR}src/crypto/bio/printf.c
  ${BORINGSSL_DIR}src/crypto/bio/socket.c
  ${BORINGSSL_DIR}src/crypto/bio/socket_helper.c
  ${BORINGSSL_DIR}src/crypto/bn_extra/bn_asn1.c
  ${BORINGSSL_DIR}src/crypto/bn_extra/convert.c
  ${BORINGSSL_DIR}src/crypto/buf/buf.c
  ${BORINGSSL_DIR}src/crypto/bytestring/asn1_compat.c
  ${BORINGSSL_DIR}src/crypto/bytestring/ber.c
  ${BORINGSSL_DIR}src/crypto/bytestring/cbb.c
  ${BORINGSSL_DIR}src/crypto/bytestring/cbs.c
  #${BORINGSSL_DIR}src/crypto/bytestring/internal.h
  ${BORINGSSL_DIR}src/crypto/bytestring/unicode.c
  ${BORINGSSL_DIR}src/crypto/chacha/chacha.c
  #${BORINGSSL_DIR}src/crypto/chacha/internal.h
  ${BORINGSSL_DIR}src/crypto/cipher_extra/cipher_extra.c
  ${BORINGSSL_DIR}src/crypto/cipher_extra/derive_key.c
  ${BORINGSSL_DIR}src/crypto/cipher_extra/e_aesccm.c
  ${BORINGSSL_DIR}src/crypto/cipher_extra/e_aesctrhmac.c
  ${BORINGSSL_DIR}src/crypto/cipher_extra/e_aesgcmsiv.c
  ${BORINGSSL_DIR}src/crypto/cipher_extra/e_chacha20poly1305.c
  ${BORINGSSL_DIR}src/crypto/cipher_extra/e_null.c
  ${BORINGSSL_DIR}src/crypto/cipher_extra/e_rc2.c
  ${BORINGSSL_DIR}src/crypto/cipher_extra/e_rc4.c
  ${BORINGSSL_DIR}src/crypto/cipher_extra/e_tls.c
  #${BORINGSSL_DIR}src/crypto/cipher_extra/internal.h
  ${BORINGSSL_DIR}src/crypto/cipher_extra/tls_cbc.c
  ${BORINGSSL_DIR}src/crypto/cmac/cmac.c
  ${BORINGSSL_DIR}src/crypto/conf/conf.c
  #${BORINGSSL_DIR}src/crypto/conf/conf_def.h
  #${BORINGSSL_DIR}src/crypto/conf/internal.h
  ${BORINGSSL_DIR}src/crypto/cpu-aarch64-fuchsia.c
  ${BORINGSSL_DIR}src/crypto/cpu-aarch64-linux.c
  ${BORINGSSL_DIR}src/crypto/cpu-arm-linux.c
  #${BORINGSSL_DIR}src/crypto/cpu-arm-linux.h
  ${BORINGSSL_DIR}src/crypto/cpu-arm.c
  ${BORINGSSL_DIR}src/crypto/cpu-intel.c
  ${BORINGSSL_DIR}src/crypto/cpu-ppc64le.c
  ${BORINGSSL_DIR}src/crypto/crypto.c
  ${BORINGSSL_DIR}src/crypto/curve25519/spake25519.c
  ${BORINGSSL_DIR}src/crypto/dh/check.c
  ${BORINGSSL_DIR}src/crypto/dh/dh.c
  ${BORINGSSL_DIR}src/crypto/dh/dh_asn1.c
  ${BORINGSSL_DIR}src/crypto/dh/params.c
  ${BORINGSSL_DIR}src/crypto/digest_extra/digest_extra.c
  ${BORINGSSL_DIR}src/crypto/dsa/dsa.c
  ${BORINGSSL_DIR}src/crypto/dsa/dsa_asn1.c
  ${BORINGSSL_DIR}src/crypto/ec_extra/ec_asn1.c
  ${BORINGSSL_DIR}src/crypto/ecdh_extra/ecdh_extra.c
  ${BORINGSSL_DIR}src/crypto/ecdsa_extra/ecdsa_asn1.c
  ${BORINGSSL_DIR}src/crypto/engine/engine.c
  ${BORINGSSL_DIR}src/crypto/err/err.c
  #${BORINGSSL_DIR}src/crypto/err/internal.h
  ${BORINGSSL_DIR}src/crypto/evp/digestsign.c
  ${BORINGSSL_DIR}src/crypto/evp/evp.c
  ${BORINGSSL_DIR}src/crypto/evp/evp_asn1.c
  ${BORINGSSL_DIR}src/crypto/evp/evp_ctx.c
  #${BORINGSSL_DIR}src/crypto/evp/internal.h
  ${BORINGSSL_DIR}src/crypto/evp/p_dsa_asn1.c
  ${BORINGSSL_DIR}src/crypto/evp/p_ec.c
  ${BORINGSSL_DIR}src/crypto/evp/p_ec_asn1.c
  ${BORINGSSL_DIR}src/crypto/evp/p_ed25519.c
  ${BORINGSSL_DIR}src/crypto/evp/p_ed25519_asn1.c
  ${BORINGSSL_DIR}src/crypto/evp/p_rsa.c
  ${BORINGSSL_DIR}src/crypto/evp/p_rsa_asn1.c
  ${BORINGSSL_DIR}src/crypto/evp/pbkdf.c
  ${BORINGSSL_DIR}src/crypto/evp/print.c
  ${BORINGSSL_DIR}src/crypto/evp/scrypt.c
  ${BORINGSSL_DIR}src/crypto/evp/sign.c
  ${BORINGSSL_DIR}src/crypto/ex_data.c
  #${BORINGSSL_DIR}src/crypto/fipsmodule/aes/internal.h
  ${BORINGSSL_DIR}src/crypto/fipsmodule/bcm.c
  #${BORINGSSL_DIR}src/crypto/fipsmodule/bn/internal.h
  #${BORINGSSL_DIR}src/crypto/fipsmodule/bn/rsaz_exp.h
  #${BORINGSSL_DIR}src/crypto/fipsmodule/cipher/internal.h
  #${BORINGSSL_DIR}src/crypto/fipsmodule/delocate.h
  #${BORINGSSL_DIR}src/crypto/fipsmodule/des/internal.h
  #${BORINGSSL_DIR}src/crypto/fipsmodule/digest/internal.h
  #${BORINGSSL_DIR}src/crypto/fipsmodule/digest/md32_common.h
  #${BORINGSSL_DIR}src/crypto/fipsmodule/ec/internal.h
  #${BORINGSSL_DIR}src/crypto/fipsmodule/ec/p256-x86_64-table.h
  #${BORINGSSL_DIR}src/crypto/fipsmodule/ec/p256-x86_64.h
  ${BORINGSSL_DIR}src/crypto/fipsmodule/is_fips.c
  #${BORINGSSL_DIR}src/crypto/fipsmodule/md5/internal.h
  #${BORINGSSL_DIR}src/crypto/fipsmodule/modes/internal.h
  #${BORINGSSL_DIR}src/crypto/fipsmodule/rand/internal.h
  #${BORINGSSL_DIR}src/crypto/fipsmodule/rsa/internal.h
  #${BORINGSSL_DIR}src/crypto/fipsmodule/sha/internal.h
  #${BORINGSSL_DIR}src/crypto/fipsmodule/tls/internal.h
  ${BORINGSSL_DIR}src/crypto/hkdf/hkdf.c
  ${BORINGSSL_DIR}src/crypto/hrss/hrss.c
  #${BORINGSSL_DIR}src/crypto/hrss/internal.h
  #${BORINGSSL_DIR}src/crypto/internal.h
  ${BORINGSSL_DIR}src/crypto/lhash/lhash.c
  ${BORINGSSL_DIR}src/crypto/mem.c
  ${BORINGSSL_DIR}src/crypto/obj/obj.c
  #${BORINGSSL_DIR}src/crypto/obj/obj_dat.h
  ${BORINGSSL_DIR}src/crypto/obj/obj_xref.c
  ${BORINGSSL_DIR}src/crypto/pem/pem_all.c
  ${BORINGSSL_DIR}src/crypto/pem/pem_info.c
  ${BORINGSSL_DIR}src/crypto/pem/pem_lib.c
  ${BORINGSSL_DIR}src/crypto/pem/pem_oth.c
  ${BORINGSSL_DIR}src/crypto/pem/pem_pk8.c
  ${BORINGSSL_DIR}src/crypto/pem/pem_pkey.c
  ${BORINGSSL_DIR}src/crypto/pem/pem_x509.c
  ${BORINGSSL_DIR}src/crypto/pem/pem_xaux.c
  #${BORINGSSL_DIR}src/crypto/pkcs7/internal.h
  ${BORINGSSL_DIR}src/crypto/pkcs7/pkcs7.c
  ${BORINGSSL_DIR}src/crypto/pkcs7/pkcs7_x509.c
  #${BORINGSSL_DIR}src/crypto/pkcs8/internal.h
  ${BORINGSSL_DIR}src/crypto/pkcs8/p5_pbev2.c
  ${BORINGSSL_DIR}src/crypto/pkcs8/pkcs8.c
  ${BORINGSSL_DIR}src/crypto/pkcs8/pkcs8_x509.c
  #${BORINGSSL_DIR}src/crypto/poly1305/internal.h
  ${BORINGSSL_DIR}src/crypto/poly1305/poly1305.c
  ${BORINGSSL_DIR}src/crypto/poly1305/poly1305_arm.c
  ${BORINGSSL_DIR}src/crypto/poly1305/poly1305_vec.c
  #${BORINGSSL_DIR}src/crypto/pool/internal.h
  ${BORINGSSL_DIR}src/crypto/pool/pool.c
  ${BORINGSSL_DIR}src/crypto/rand_extra/deterministic.c
  ${BORINGSSL_DIR}src/crypto/rand_extra/forkunsafe.c
  ${BORINGSSL_DIR}src/crypto/rand_extra/fuchsia.c
  ${BORINGSSL_DIR}src/crypto/rand_extra/rand_extra.c
  ${BORINGSSL_DIR}src/crypto/rand_extra/windows.c
  ${BORINGSSL_DIR}src/crypto/rc4/rc4.c
  ${BORINGSSL_DIR}src/crypto/refcount_c11.c
  ${BORINGSSL_DIR}src/crypto/refcount_lock.c
  ${BORINGSSL_DIR}src/crypto/rsa_extra/rsa_asn1.c
  ${BORINGSSL_DIR}src/crypto/rsa_extra/rsa_print.c
  ${BORINGSSL_DIR}src/crypto/stack/stack.c
  ${BORINGSSL_DIR}src/crypto/thread.c
  ${BORINGSSL_DIR}src/crypto/thread_none.c
  ${BORINGSSL_DIR}src/crypto/thread_pthread.c
  ${BORINGSSL_DIR}src/crypto/thread_win.c
  ${BORINGSSL_DIR}src/crypto/x509/a_digest.c
  ${BORINGSSL_DIR}src/crypto/x509/a_sign.c
  ${BORINGSSL_DIR}src/crypto/x509/a_strex.c
  ${BORINGSSL_DIR}src/crypto/x509/a_verify.c
  ${BORINGSSL_DIR}src/crypto/x509/algorithm.c
  ${BORINGSSL_DIR}src/crypto/x509/asn1_gen.c
  ${BORINGSSL_DIR}src/crypto/x509/by_dir.c
  ${BORINGSSL_DIR}src/crypto/x509/by_file.c
  #${BORINGSSL_DIR}src/crypto/x509/charmap.h
  ${BORINGSSL_DIR}src/crypto/x509/i2d_pr.c
  #${BORINGSSL_DIR}src/crypto/x509/internal.h
  ${BORINGSSL_DIR}src/crypto/x509/rsa_pss.c
  ${BORINGSSL_DIR}src/crypto/x509/t_crl.c
  ${BORINGSSL_DIR}src/crypto/x509/t_req.c
  ${BORINGSSL_DIR}src/crypto/x509/t_x509.c
  ${BORINGSSL_DIR}src/crypto/x509/t_x509a.c
  #${BORINGSSL_DIR}src/crypto/x509/vpm_int.h
  ${BORINGSSL_DIR}src/crypto/x509/x509.c
  ${BORINGSSL_DIR}src/crypto/x509/x509_att.c
  ${BORINGSSL_DIR}src/crypto/x509/x509_cmp.c
  ${BORINGSSL_DIR}src/crypto/x509/x509_d2.c
  ${BORINGSSL_DIR}src/crypto/x509/x509_def.c
  ${BORINGSSL_DIR}src/crypto/x509/x509_ext.c
  ${BORINGSSL_DIR}src/crypto/x509/x509_lu.c
  ${BORINGSSL_DIR}src/crypto/x509/x509_obj.c
  ${BORINGSSL_DIR}src/crypto/x509/x509_r2x.c
  ${BORINGSSL_DIR}src/crypto/x509/x509_req.c
  ${BORINGSSL_DIR}src/crypto/x509/x509_set.c
  ${BORINGSSL_DIR}src/crypto/x509/x509_trs.c
  ${BORINGSSL_DIR}src/crypto/x509/x509_txt.c
  ${BORINGSSL_DIR}src/crypto/x509/x509_v3.c
  ${BORINGSSL_DIR}src/crypto/x509/x509_vfy.c
  ${BORINGSSL_DIR}src/crypto/x509/x509_vpm.c
  ${BORINGSSL_DIR}src/crypto/x509/x509cset.c
  ${BORINGSSL_DIR}src/crypto/x509/x509name.c
  ${BORINGSSL_DIR}src/crypto/x509/x509rset.c
  ${BORINGSSL_DIR}src/crypto/x509/x509spki.c
  ${BORINGSSL_DIR}src/crypto/x509/x_algor.c
  ${BORINGSSL_DIR}src/crypto/x509/x_all.c
  ${BORINGSSL_DIR}src/crypto/x509/x_attrib.c
  ${BORINGSSL_DIR}src/crypto/x509/x_crl.c
  ${BORINGSSL_DIR}src/crypto/x509/x_exten.c
  ${BORINGSSL_DIR}src/crypto/x509/x_info.c
  ${BORINGSSL_DIR}src/crypto/x509/x_name.c
  ${BORINGSSL_DIR}src/crypto/x509/x_pkey.c
  ${BORINGSSL_DIR}src/crypto/x509/x_pubkey.c
  ${BORINGSSL_DIR}src/crypto/x509/x_req.c
  ${BORINGSSL_DIR}src/crypto/x509/x_sig.c
  ${BORINGSSL_DIR}src/crypto/x509/x_spki.c
  ${BORINGSSL_DIR}src/crypto/x509/x_val.c
  ${BORINGSSL_DIR}src/crypto/x509/x_x509.c
  ${BORINGSSL_DIR}src/crypto/x509/x_x509a.c
  #${BORINGSSL_DIR}src/crypto/x509v3/ext_dat.h
  #${BORINGSSL_DIR}src/crypto/x509v3/internal.h
  ${BORINGSSL_DIR}src/crypto/x509v3/pcy_cache.c
  ${BORINGSSL_DIR}src/crypto/x509v3/pcy_data.c
  #${BORINGSSL_DIR}src/crypto/x509v3/pcy_int.h
  ${BORINGSSL_DIR}src/crypto/x509v3/pcy_lib.c
  ${BORINGSSL_DIR}src/crypto/x509v3/pcy_map.c
  ${BORINGSSL_DIR}src/crypto/x509v3/pcy_node.c
  ${BORINGSSL_DIR}src/crypto/x509v3/pcy_tree.c
  ${BORINGSSL_DIR}src/crypto/x509v3/v3_akey.c
  ${BORINGSSL_DIR}src/crypto/x509v3/v3_akeya.c
  ${BORINGSSL_DIR}src/crypto/x509v3/v3_alt.c
  ${BORINGSSL_DIR}src/crypto/x509v3/v3_bcons.c
  ${BORINGSSL_DIR}src/crypto/x509v3/v3_bitst.c
  ${BORINGSSL_DIR}src/crypto/x509v3/v3_conf.c
  ${BORINGSSL_DIR}src/crypto/x509v3/v3_cpols.c
  ${BORINGSSL_DIR}src/crypto/x509v3/v3_crld.c
  ${BORINGSSL_DIR}src/crypto/x509v3/v3_enum.c
  ${BORINGSSL_DIR}src/crypto/x509v3/v3_extku.c
  ${BORINGSSL_DIR}src/crypto/x509v3/v3_genn.c
  ${BORINGSSL_DIR}src/crypto/x509v3/v3_ia5.c
  ${BORINGSSL_DIR}src/crypto/x509v3/v3_info.c
  ${BORINGSSL_DIR}src/crypto/x509v3/v3_int.c
  ${BORINGSSL_DIR}src/crypto/x509v3/v3_lib.c
  ${BORINGSSL_DIR}src/crypto/x509v3/v3_ncons.c
  ${BORINGSSL_DIR}src/crypto/x509v3/v3_ocsp.c
  ${BORINGSSL_DIR}src/crypto/x509v3/v3_pci.c
  ${BORINGSSL_DIR}src/crypto/x509v3/v3_pcia.c
  ${BORINGSSL_DIR}src/crypto/x509v3/v3_pcons.c
  ${BORINGSSL_DIR}src/crypto/x509v3/v3_pku.c
  ${BORINGSSL_DIR}src/crypto/x509v3/v3_pmaps.c
  ${BORINGSSL_DIR}src/crypto/x509v3/v3_prn.c
  ${BORINGSSL_DIR}src/crypto/x509v3/v3_purp.c
  ${BORINGSSL_DIR}src/crypto/x509v3/v3_skey.c
  ${BORINGSSL_DIR}src/crypto/x509v3/v3_sxnet.c
  ${BORINGSSL_DIR}src/crypto/x509v3/v3_utl.c
  ${BORINGSSL_DIR}src/third_party/fiat/curve25519.c
  #${BORINGSSL_DIR}src/third_party/fiat/curve25519_32.h
  #${BORINGSSL_DIR}src/third_party/fiat/curve25519_64.h
  #${BORINGSSL_DIR}src/third_party/fiat/curve25519_tables.h
  #${BORINGSSL_DIR}src/third_party/fiat/internal.h
  #${BORINGSSL_DIR}src/third_party/fiat/p256_32.h
  #${BORINGSSL_DIR}src/third_party/fiat/p256_64.h
)

add_library(BORINGSSL STATIC
  ${BORINGSSL_SOURCES}
)

target_link_libraries(BORINGSSL PUBLIC
  dynamic_annotations
  #${BASE_LIBRARIES}
  base
)

set_property(TARGET BORINGSSL PROPERTY CXX_STANDARD 17)

target_include_directories(BORINGSSL PRIVATE
  ${BORINGSSL_DIR}
  ${BASE_DIR}
  ${BORINGSSL_DIR}src/
)

target_include_directories(BORINGSSL PUBLIC
  ${BORINGSSL_DIR}src/include/
  ${BORINGSSL_DIR}src/include/openssl
)

target_compile_definitions(BORINGSSL PRIVATE
  #BORINGSSL_SHARED_LIBRARY=1
  #BORINGSSL_ALLOW_CXX_RUNTIME
  BORINGSSL_IMPLEMENTATION=1
  BORINGSSL_NO_STATIC_INITIALIZER=1
  # OPENSSL_NO_ASM prevents the use of assembly code
  OPENSSL_NO_ASM=1
  # OPENSSL_SMALL removes some code that is especially large at some performance cost.
  # see https://boringssl.googlesource.com/boringssl/+/HEAD/INCORPORATING.md
  OPENSSL_SMALL=1
  # -std=c99
  _XOPEN_SOURCE=700 # is_posix || is_fuchsia
)
