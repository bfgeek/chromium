# This file is used to manage the dependencies of the Chromium src repo. It is
# used by gclient to determine what version of each dependency to check out, and
# where.
#
# For more information, please refer to the official documentation:
#   https://sites.google.com/a/chromium.org/dev/developers/how-tos/get-the-code
#
# When adding a new dependency, please update the top-level .gitignore file
# to list the dependency's destination directory.
#
# -----------------------------------------------------------------------------
# Rolling deps
# -----------------------------------------------------------------------------
# All repositories in this file are git-based, using Chromium git mirrors where
# necessary (e.g., a git mirror is used when the source project is SVN-based).
# To update the revision that Chromium pulls for a given dependency:
#
#  # Create and switch to a new branch
#  git new-branch depsroll
#  # Run roll-dep (provided by depot_tools) giving the dep's path and optionally
#  # a regex that will match the line in this file that contains the current
#  # revision. The script ALWAYS rolls the dependency to the latest revision
#  # in origin/master. The path for the dep should start with src/.
#  roll-dep src/third_party/foo_package/src foo_package.git
#  # You should now have a modified DEPS file; commit and upload as normal
#  git commit -a
#  git cl upload


vars = {
  # Use this googlecode_url variable only if there is an internal mirror for it.
  # If you do not know, use the full path while defining your new deps entry.
  'googlecode_url': 'http://%s.googlecode.com/svn',
  'chromium_git': 'https://chromium.googlesource.com',
  # Three lines of non-changing comments so that
  # the commit queue can handle CLs rolling sfntly
  # and whatever else without interference from each other.
  'sfntly_revision': '130f832eddf98467e6578b548cb74ce17d04a26d',
  # Three lines of non-changing comments so that
  # the commit queue can handle CLs rolling Skia
  # and whatever else without interference from each other.
  'skia_revision': '1437c1eea8f40f7a43bfe6e80a8d3f6cd61a3643',
  # Three lines of non-changing comments so that
  # the commit queue can handle CLs rolling V8
  # and whatever else without interference from each other.
  'v8_revision': '892061f29f6bd61c923d86769dad0b0d3f5aef43',
  # Three lines of non-changing comments so that
  # the commit queue can handle CLs rolling swarming_client
  # and whatever else without interference from each other.
  'swarming_revision': 'df6e95e7669883c8fe9ef956c69a544154701a49',
  # Three lines of non-changing comments so that
  # the commit queue can handle CLs rolling ANGLE
  # and whatever else without interference from each other.
  'angle_revision': 'fc4712b5ed270436f2993bfda9e916d4f92684a4',
  # Three lines of non-changing comments so that
  # the commit queue can handle CLs rolling build tools
  # and whatever else without interference from each other.
  'buildtools_revision': '7274f1c726640bce4cd4c4198e1ee62a0b29bb89',
  # Three lines of non-changing comments so that
  # the commit queue can handle CLs rolling PDFium
  # and whatever else without interference from each other.
  'pdfium_revision': '55265016faac358266af280db6c62afa34ce2891',
  # Three lines of non-changing comments so that
  # the commit queue can handle CLs rolling openmax_dl
  # and whatever else without interference from each other.
  'openmax_dl_revision': '6670e52d32351145a6b6c198dab3f6a536edf3db',
  # Three lines of non-changing comments so that
  # the commit queue can handle CLs rolling BoringSSL
  # and whatever else without interference from each other.
  'boringssl_revision': 'a857159dd61204bfe93bd8e2f00448434e8b0b99',
  # Three lines of non-changing comments so that
  # the commit queue can handle CLs rolling nss
  # and whatever else without interference from each other.
  'nss_revision': '225bfc39c93dfb7c7d0d1162f81e9bb5cd356c30',
  # Three lines of non-changing comments so that
  # the commit queue can handle CLs rolling google-toolbox-for-mac
  # and whatever else without interference from each other.
  'google_toolbox_for_mac_revision': '401878398253074c515c03cb3a3f8bb0cc8da6e9',
  # Three lines of non-changing comments so that
  # the commit queue can handle CLs rolling lighttpd
  # and whatever else without interference from each other.
  'lighttpd_revision': '9dfa55d15937a688a92cbf2b7a8621b0927d06eb',
  # Three lines of non-changing comments so that
  # the commit queue can handle CLs rolling lss
  # and whatever else without interference from each other.
  'lss_revision': '4fc942258fe5509549333b9487ec018e3c8c5b10',
  # Three lines of non-changing comments so that
  # the commit queue can handle CLs rolling NaCl
  # and whatever else without interference from each other.
  'nacl_revision': 'bf567db351bfe01e6ccc6adce04c2c1ff9f9daee',
  # Three lines of non-changing comments so that
  # the commit queue can handle CLs rolling dEQP
  # and whatever else without interference from each other.
  'deqp_revision': 'cc0ded6c77267bbb14d21aac358fc5d9690c07f8',
  'deqp_url': 'https://android.googlesource.com/platform/external/deqp',
  # Three lines of non-changing comments so that
  # the commit queue can handle CLs rolling freetype-android
  # and whatever else without interference from each other.
  'freetype_android_revision': 'a512b0fe7a8d9db0e5aa9c0a4db1e92cb861722d',
}

# Only these hosts are allowed for dependencies in this DEPS file.
# If you need to add a new host, contact chrome infrastracture team.
allowed_hosts = [
  'chromium.googlesource.com',
  'boringssl.googlesource.com',
  'pdfium.googlesource.com',
  'android.googlesource.com',
]

deps = {
  'src/breakpad/src':
   Var('chromium_git') + '/breakpad/breakpad/src.git' + '@' + '481608d284106df9400d447e95574799670cabaf',

  'src/buildtools':
   Var('chromium_git') + '/chromium/buildtools.git' + '@' +  Var('buildtools_revision'),

  'src/sdch/open-vcdiff':
   Var('chromium_git') + '/external/github.com/google/open-vcdiff.git' + '@' + '21d7d0b9c3d0c3ccbdb221c85ae889373f0a2a58',

  'src/testing/gtest':
   Var('chromium_git') + '/external/github.com/google/googletest.git' + '@' + '6f8a66431cb592dad629028a50b3dd418a408c87',

  'src/testing/gmock':
   Var('chromium_git') + '/external/googlemock.git' + '@' + '0421b6f358139f02e102c9c332ce19a33faf75be', # from svn revision 566

  'src/third_party/angle':
   Var('chromium_git') + '/angle/angle.git' + '@' +  Var('angle_revision'),

  'src/third_party/colorama/src':
   Var('chromium_git') + '/external/colorama.git' + '@' + '799604a1041e9b3bc5d2789ecbd7e8db2e18e6b8',

  'src/third_party/icu':
   Var('chromium_git') + '/chromium/deps/icu.git' + '@' + '6d3b55dfa17d9a597c064eeef7ceac18c73184b7',

  'src/third_party/libexif/sources':
   Var('chromium_git') + '/chromium/deps/libexif/sources.git' + '@' + '9d467f7d21e4749ee22ee7520e561ac7b38484b9',

  'src/third_party/hunspell_dictionaries':
   Var('chromium_git') + '/chromium/deps/hunspell_dictionaries.git' + '@' + 'c106afdcec5d3de2622e19f1b3294c47bbd8bd72',

  'src/third_party/safe_browsing/testing':
    Var('chromium_git') + '/external/google-safe-browsing/testing.git' + '@' + '9d7e8064f3ca2e45891470c9b5b1dce54af6a9d6',

  'src/third_party/leveldatabase/src':
    Var('chromium_git') + '/external/leveldb.git' + '@' + '7306ef856a91e462a73ff1832c1fa8771008ba36',

  'src/third_party/snappy/src':
    Var('chromium_git') + '/external/snappy.git' + '@' + '762bb32f0c9d2f31ba4958c7c0933d22e80c20bf',

  'src/tools/gyp':
    Var('chromium_git') + '/external/gyp.git' + '@' + '61259d585ce99d9de8b35188cd0025bdbcb58411',

  'src/tools/swarming_client':
   Var('chromium_git') + '/external/swarming.client.git' + '@' +  Var('swarming_revision'),

  'src/v8':
    Var('chromium_git') + '/v8/v8.git' + '@' +  Var('v8_revision'),

  'src/native_client':
   Var('chromium_git') + '/native_client/src/native_client.git' + '@' + Var('nacl_revision'),

  'src/third_party/sfntly/src':
   Var('chromium_git') + '/external/github.com/googlei18n/sfntly.git' + '@' + Var('sfntly_revision'),

  'src/third_party/skia':
   Var('chromium_git') + '/skia.git' + '@' +  Var('skia_revision'),

  'src/tools/page_cycler/acid3':
   Var('chromium_git') + '/chromium/deps/acid3.git' + '@' + '6be0a66a1ebd7ebc5abc1b2f405a945f6d871521',

  'src/chrome/test/data/perf/canvas_bench':
   Var('chromium_git') + '/chromium/canvas_bench.git' + '@' + 'a7b40ea5ae0239517d78845a5fc9b12976bfc732',

  'src/chrome/test/data/perf/frame_rate/content':
   Var('chromium_git') + '/chromium/frame_rate/content.git' + '@' + 'c10272c88463efeef6bb19c9ec07c42bc8fe22b9',

  'src/third_party/bidichecker':
    Var('chromium_git') + '/external/bidichecker/lib.git' + '@' + '97f2aa645b74c28c57eca56992235c79850fa9e0',

  'src/third_party/webgl/src':
   Var('chromium_git') + '/external/khronosgroup/webgl.git' + '@' + '1012e1f8baea808f3bc9fcef945d66b5f740c32b',

  'src/third_party/webdriver/pylib':
    Var('chromium_git') + '/external/selenium/py.git' + '@' + '5fd78261a75fe08d27ca4835fb6c5ce4b42275bd',

  'src/third_party/libvpx/source/libvpx':
   Var('chromium_git') + '/webm/libvpx.git' + '@' +  '9aa083d164e0d39086aa0c83f0d1a0d0f0d1ba61',

  'src/third_party/ffmpeg':
   Var('chromium_git') + '/chromium/third_party/ffmpeg.git' + '@' + 'beef68513861e11156899b1f763097c31808c418',

  'src/third_party/libjingle/source/talk':
    Var('chromium_git') + '/external/webrtc/trunk/talk.git' + '@' + 'fedf40913e5ca2904d533c2ac67b806b3ea191aa', # commit position 11936

  'src/third_party/usrsctp/usrsctplib':
    Var('chromium_git') + '/external/github.com/sctplab/usrsctp' + '@' + 'c60ec8b35c3fe6027d7a3faae89d1c8d7dd3ce98',

  'src/third_party/libsrtp':
   Var('chromium_git') + '/chromium/deps/libsrtp.git' + '@' + '8eecac0feef4c65e2debb42718a10eab91551f35', # from svn revision 295151

  'src/third_party/yasm/source/patched-yasm':
   Var('chromium_git') + '/chromium/deps/yasm/patched-yasm.git' + '@' + '7da28c6c7c6a1387217352ce02b31754deb54d2a',

  'src/third_party/libjpeg_turbo':
   Var('chromium_git') + '/chromium/deps/libjpeg_turbo.git' + '@' + 'e4e75037f29745f1546b6ebf5cf532e841c04c2c',

  'src/third_party/flac':
   Var('chromium_git') + '/chromium/deps/flac.git' + '@' + '2c4b86af352b23498315c016dc207e3fb2733fc0',

  'src/third_party/pyftpdlib/src':
    Var('chromium_git') + '/external/pyftpdlib.git' + '@' + '2be6d65e31c7ee6320d059f581f05ae8d89d7e45',

  'src/third_party/scons-2.0.1':
   Var('chromium_git') + '/native_client/src/third_party/scons-2.0.1.git' + '@' + '1c1550e17fc26355d08627fbdec13d8291227067',

  'src/third_party/webrtc':
    Var('chromium_git') + '/external/webrtc/trunk/webrtc.git' + '@' + '2fae8b30e91f07e705f8ee6162b16b47fc2300ee', # commit position 11965

  'src/third_party/openmax_dl':
    Var('chromium_git') + '/external/webrtc/deps/third_party/openmax.git' + '@' +  Var('openmax_dl_revision'),

  'src/third_party/jsoncpp/source':
    Var('chromium_git') + '/external/github.com/open-source-parsers/jsoncpp.git' + '@' + 'f572e8e42e22cfcf5ab0aea26574f408943edfa4', # from svn 248

  'src/third_party/libyuv':
    Var('chromium_git') + '/libyuv/libyuv.git' + '@' + '20343f45c612e485cd898aeaae2250df2d0b2d2d', # from version 1576

  'src/third_party/smhasher/src':
    Var('chromium_git') + '/external/smhasher.git' + '@' + 'e87738e57558e0ec472b2fc3a643b838e5b6e88f',

  'src/third_party/libaddressinput/src':
    Var('chromium_git') + '/external/libaddressinput.git' + '@' + '5eeeb797e79fa01503fcdcbebdc50036fac023ef',

  'src/third_party/libphonenumber/dist':
    Var('chromium_git') + '/external/libphonenumber.git' + '@' + 'a9bd368debe74355364965c24bcc5a6bfb1dc546',

  'src/third_party/webpagereplay':
   Var('chromium_git') + '/external/github.com/chromium/web-page-replay.git' + '@' + '7564939bdf6482d57b9bd5e9c931679f96d8cf75',

  'src/third_party/pywebsocket/src':
    Var('chromium_git') + '/external/github.com/google/pywebsocket.git' + '@' + '2d7b73c3acbd0f41dcab487ae5c97c6feae06ce2',

  'src/third_party/opus/src':
   Var('chromium_git') + '/chromium/deps/opus.git' + '@' + '655cc54c564b84ef2827f0b2152ce3811046201e',

  'src/media/cdm/api':
   Var('chromium_git') + '/chromium/cdm.git' + '@' + '1dea7088184dec2ebe4a8b3800aabb0afbb4b88a',

  'src/third_party/mesa/src':
   Var('chromium_git') + '/chromium/deps/mesa.git' + '@' + 'ef811c6bd4de74e13e7035ca882cc77f85793fef',

  'src/third_party/cld_2/src':
    Var('chromium_git') + '/external/github.com/CLD2Owners/cld2.git' + '@' + '84b58a5d7690ebf05a91406f371ce00c3daf31c0',

  'src/third_party/libwebm/source':
   Var('chromium_git') + '/webm/libwebm.git' + '@' + '75a6d2da8b63e0c446ec0ce1ac942c2962d959d7',

  'src/third_party/pdfium':
   'https://pdfium.googlesource.com/pdfium.git' + '@' +  Var('pdfium_revision'),

  'src/third_party/boringssl/src':
   'https://boringssl.googlesource.com/boringssl.git' + '@' +  Var('boringssl_revision'),

  'src/third_party/py_trace_event/src':
    Var('chromium_git') + '/external/py_trace_event.git' + '@' + 'dd463ea9e2c430de2b9e53dea57a77b4c3ac9b30',

  'src/third_party/dom_distiller_js/dist':
    Var('chromium_git') + '/external/github.com/chromium/dom-distiller-dist.git' + '@' + 'e21fe06cb71327ec62431f823e783d7b02f97b26',

  'src/third_party/catapult':
    Var('chromium_git') + '/external/github.com/catapult-project/catapult.git' + '@' +
    'a0b4038efb78eeebadeefa8fe71348cdaa42bd90',

  'src/third_party/openh264/src':
    Var('chromium_git') + '/external/github.com/cisco/openh264' + '@' + 'b37cda248234162033e3e11b0335f3131cdfe488',

  'src/third_party/re2/src':
    Var('chromium_git') + '/external/github.com/google/re2.git' + '@' + 'dba3349aba83b5588e85e5ecf2b56c97f2d259b7',
}


deps_os = {
  'win': {
    'src/chrome/tools/test/reference_build/chrome_win':
     Var('chromium_git') + '/chromium/reference_builds/chrome_win.git' + '@' + 'f8a3a845dfc845df6b14280f04f86a61959357ef',

    'src/third_party/cygwin':
     Var('chromium_git') + '/chromium/deps/cygwin.git' + '@' + 'c89e446b273697fadf3a10ff1007a97c0b7de6df',

    'src/third_party/psyco_win32':
     Var('chromium_git') + '/chromium/deps/psyco_win32.git' + '@' + 'f5af9f6910ee5a8075bbaeed0591469f1661d868',

    'src/third_party/bison':
     Var('chromium_git') + '/chromium/deps/bison.git' + '@' + '083c9a45e4affdd5464ee2b224c2df649c6e26c3',

    'src/third_party/gperf':
     Var('chromium_git') + '/chromium/deps/gperf.git' + '@' + 'd892d79f64f9449770443fb06da49b5a1e5d33c1',

    'src/third_party/perl':
     Var('chromium_git') + '/chromium/deps/perl.git' + '@' + 'ac0d98b5cee6c024b0cffeb4f8f45b6fc5ccdb78',

    'src/third_party/lighttpd':
     Var('chromium_git') + '/chromium/deps/lighttpd.git' + '@' + Var('lighttpd_revision'),

    # Parses Windows PE/COFF executable format.
    'src/third_party/pefile':
     Var('chromium_git') + '/external/pefile.git' + '@' + '72c6ae42396cb913bcab63c15585dc3b5c3f92f1',

    # NSS, for SSLClientSocketNSS.
    'src/third_party/nss':
     Var('chromium_git') + '/chromium/deps/nss.git' + '@' + Var('nss_revision'),

    # GNU binutils assembler for x86-32.
    'src/third_party/gnu_binutils':
      Var('chromium_git') + '/native_client/deps/third_party/gnu_binutils.git' + '@' + 'f4003433b61b25666565690caf3d7a7a1a4ec436',
    # GNU binutils assembler for x86-64.
    'src/third_party/mingw-w64/mingw/bin':
      Var('chromium_git') + '/native_client/deps/third_party/mingw-w64/mingw/bin.git' + '@' + '3cc8b140b883a9fe4986d12cfd46c16a093d3527',

    # Dependencies used by libjpeg-turbo
    'src/third_party/yasm/binaries':
     Var('chromium_git') + '/chromium/deps/yasm/binaries.git' + '@' + '52f9b3f4b0aa06da24ef8b123058bb61ee468881',

    # Binaries for nacl sdk.
    'src/third_party/nacl_sdk_binaries':
     Var('chromium_git') + '/chromium/deps/nacl_sdk_binaries.git' + '@' + '759dfca03bdc774da7ecbf974f6e2b84f43699a5',

    # ANGLE uses dEQP for GPU testing
    'src/third_party/deqp/src':
     Var('deqp_url') + '@' + Var('deqp_revision'),
  },
  'ios': {
    'src/ios/third_party/earl_grey/src':
     Var('chromium_git') + '/external/github.com/google/EarlGrey.git' + '@' + 'ffdbbb7e9e0a3c617d9f110ef752dbd2dc09c919',

    'src/ios/third_party/fishhook/src':
     Var('chromium_git') + '/external/github.com/facebook/fishhook.git' + '@' + 'd172d5247aa590c25d0b1885448bae76036ea22c',

    'src/ios/third_party/gcdwebserver/src':
     Var('chromium_git') + '/external/github.com/swisspol/GCDWebServer.git' + '@' + '3d5fd0b8281a7224c057deb2d17709b5bea64836',

    'src/ios/third_party/ochamcrest/src':
     Var('chromium_git') + '/external/github.com/hamcrest/OCHamcrest.git' + '@' + 'd7ee4ecfb6bd13c3c8d364682b6228ccd86e1e1a',

    'src/third_party/google_toolbox_for_mac/src':
      Var('chromium_git') + '/external/github.com/google/google-toolbox-for-mac.git' + '@' + Var('google_toolbox_for_mac_revision'),

    'src/third_party/nss':
     Var('chromium_git') + '/chromium/deps/nss.git' + '@' + Var('nss_revision'),

    # class-dump utility to generate header files for undocumented SDKs
    'src/third_party/class-dump/src':
     Var('chromium_git') + '/external/github.com/nygard/class-dump.git' + '@' + '978d177ca6f0d2e5e34acf3e8dadc63e3140ebbc',

    # Code that's not needed due to not building everything
    'src/chrome/test/data/perf/canvas_bench': None,
    'src/chrome/test/data/perf/frame_rate/content': None,
    'src/native_client': None,
    'src/third_party/ffmpeg': None,
    'src/third_party/hunspell_dictionaries': None,
    'src/third_party/webgl': None,
  },
  'mac': {
    'src/chrome/tools/test/reference_build/chrome_mac':
     Var('chromium_git') + '/chromium/reference_builds/chrome_mac.git' + '@' + '8dc181329e7c5255f83b4b85dc2f71498a237955',

    'src/third_party/google_toolbox_for_mac/src':
      Var('chromium_git') + '/external/github.com/google/google-toolbox-for-mac.git' + '@' + Var('google_toolbox_for_mac_revision'),


    'src/third_party/pdfsqueeze':
      Var('chromium_git') + '/external/pdfsqueeze.git' + '@' + '5936b871e6a087b7e50d4cbcb122378d8a07499f',

    'src/third_party/lighttpd':
     Var('chromium_git') + '/chromium/deps/lighttpd.git' + '@' + Var('lighttpd_revision'),

    # NSS, for SSLClientSocketNSS.
    'src/third_party/nss':
     Var('chromium_git') + '/chromium/deps/nss.git' + '@' + Var('nss_revision'),

    'src/chrome/installer/mac/third_party/xz/xz':
     Var('chromium_git') + '/chromium/deps/xz.git' + '@' + 'eecaf55632ca72e90eb2641376bce7cdbc7284f7',
  },
  'unix': {
    # Linux, really.
    'src/chrome/tools/test/reference_build/chrome_linux':
     Var('chromium_git') + '/chromium/reference_builds/chrome_linux64.git' + '@' + '033d053a528e820e1de3e2db766678d862a86b36',

    'src/third_party/xdg-utils':
     Var('chromium_git') + '/chromium/deps/xdg-utils.git' + '@' + 'd80274d5869b17b8c9067a1022e4416ee7ed5e0d',

    'src/third_party/lss':
      Var('chromium_git') + '/external/linux-syscall-support/lss.git' + '@' + Var('lss_revision'),

    # For Linux and Chromium OS.
    'src/third_party/cros_system_api':
     Var('chromium_git') + '/chromiumos/platform/system_api.git' + '@' + '8e2e37df9c5326a8e4198553a4befb0c1be97841',

    # Note that this is different from Android's freetype repo.
    'src/third_party/freetype2/src':
     Var('chromium_git') + '/chromium/src/third_party/freetype2.git' + '@' + 'fc1532a7c4c592f24a4c1a0261d2845524ca5cff',

    'src/third_party/freetype-android/src':
     Var('chromium_git') + '/chromium/src/third_party/freetype2.git' + '@' + Var('freetype_android_revision'),

    # Build tools for Chrome OS.
    'src/third_party/chromite':
     Var('chromium_git') + '/chromiumos/chromite.git' + '@' + 'e19f83ba227bf1ec0077f5d3a816a415f1dd88d0',

    # Dependency of chromite.git.
    'src/third_party/pyelftools':
     Var('chromium_git') + '/chromiumos/third_party/pyelftools.git' + '@' + 'bdc1d380acd88d4bfaf47265008091483b0d614e',

    'src/third_party/liblouis/src':
     Var('chromium_git') + '/external/liblouis-github.git' + '@' + '5f9c03f2a3478561deb6ae4798175094be8a26c2',

    # Used for embedded builds. CrOS & Linux use the system version.
    'src/third_party/fontconfig/src':
     Var('chromium_git') + '/external/fontconfig.git' + '@' + 'f16c3118e25546c1b749f9823c51827a60aeb5c1',

    # ANGLE uses dEQP for GPU testing
    'src/third_party/deqp/src':
     Var('deqp_url') + '@' + Var('deqp_revision'),

    # Graphics buffer allocator for Chrome OS.
    'src/third_party/minigbm/src':
     Var('chromium_git') + '/chromiumos/platform/minigbm.git' + '@' + '1e229399228ac107e8ae0f73a466e3ea17fe5839',

    # Display server protocol for Linux.
    'src/third_party/wayland/src':
     Var('chromium_git') + '/external/anongit.freedesktop.org/git/wayland/wayland.git' + '@' + '7ed00c1de77afbab23f4908fbd9d60ec070c209b',

    # Wayland protocols that add functionality not available in the core protocol.
    'src/third_party/wayland-protocols/src':
     Var('chromium_git') + '/external/anongit.freedesktop.org/git/wayland/wayland-protocols.git' + '@' + '596dfda882a51c05699bcb28a8459ce936a138db',

    # Wireless Display Software. Used on Chrome OS.
    'src/third_party/wds/src':
     Var('chromium_git') + '/external/github.com/01org/wds' + '@' + 'f187dda5fccaad08e168dc6657109325f42c648e',
  },
  'android': {
    'src/third_party/android_protobuf/src':
     Var('chromium_git') + '/external/android_protobuf.git' + '@' + '999188d0dc72e97f7fe08bb756958a2cf090f4e7',

    'src/third_party/android_tools':
     Var('chromium_git') + '/android_tools.git' + '@' + 'f4c36ad89b2696b37d9cd7ca7d984b691888b188',

    'src/third_party/apache-mime4j':
     Var('chromium_git') + '/chromium/deps/apache-mime4j.git' + '@' + '28cb1108bff4b6cf0a2e86ff58b3d025934ebe3a',

    'src/third_party/apache-portable-runtime/src':
     Var('chromium_git') + '/external/apache-portable-runtime.git' + '@' + 'c76a8c4277e09a82eaa229e35246edea1ee0a6a1',

    'src/third_party/appurify-python/src':
     Var('chromium_git') + '/external/github.com/appurify/appurify-python.git' + '@' + 'ee7abd5c5ae3106f72b2a0b9d2cb55094688e867',

    'src/third_party/cardboard-java/src':
     Var('chromium_git') + '/external/github.com/googlesamples/cardboard-java.git' + '@' + 'e36ee57e72bbd057ddb53b127954177b50e18df7',

    'src/third_party/errorprone/lib':
      Var('chromium_git') + '/chromium/third_party/errorprone.git' + '@' + '0eea83b66343133b9c76b7d3288c30321818ebcf',

    'src/third_party/findbugs':
     Var('chromium_git') + '/chromium/deps/findbugs.git' + '@' + '57f05238d3ac77ea0a194813d3065dd780c6e566',

    'src/third_party/freetype-android/src':
     Var('chromium_git') + '/chromium/src/third_party/freetype2.git' + '@' + Var('freetype_android_revision'),

    'src/third_party/elfutils/src':
     Var('chromium_git') + '/external/elfutils.git' + '@' + '249673729a7e5dbd5de4f3760bdcaa3d23d154d7',

    'src/third_party/httpcomponents-client':
     Var('chromium_git') + '/chromium/deps/httpcomponents-client.git' + '@' + '285c4dafc5de0e853fa845dce5773e223219601c',

    'src/third_party/httpcomponents-core':
     Var('chromium_git') + '/chromium/deps/httpcomponents-core.git' + '@' + '9f7180a96f8fa5cab23f793c14b413356d419e62',

    'src/third_party/jarjar':
     Var('chromium_git') + '/chromium/deps/jarjar.git' + '@' + '2e1ead4c68c450e0b77fe49e3f9137842b8b6920',

    'src/third_party/jsr-305/src':
      Var('chromium_git') + '/external/jsr-305.git' + '@' + '642c508235471f7220af6d5df2d3210e3bfc0919',

    'src/third_party/junit/src':
      Var('chromium_git') + '/external/junit.git' + '@' + '45a44647e7306262162e1346b750c3209019f2e1',

    'src/third_party/mockito/src':
      Var('chromium_git') + '/external/mockito/mockito.git' + '@' + '4d987dcd923b81525c42b1333e6c4e07440776c3',

    'src/third_party/netty-tcnative/src':
      Var('chromium_git') + '/external/netty-tcnative.git' + '@' + '12d01332921695e974175870175eb14a889313a1',

    'src/third_party/netty4/src':
      Var('chromium_git') + '/external/netty4.git' + '@' + 'e0f26303b4ce635365be19414d0ac81f2ef6ba3c',

    'src/third_party/robolectric/lib':
      Var('chromium_git') + '/chromium/third_party/robolectric.git' + '@' + '6b63c99a8b6967acdb42cbed0adb067c80efc810',

    'src/third_party/ub-uiautomator/lib':
      Var('chromium_git') + '/chromium/third_party/ub-uiautomator.git' + '@' + '00270549ce3161ae72ceb24712618ea28b4f9434',

    'src/third_party/leakcanary/src':
      Var('chromium_git') + '/external/github.com/square/leakcanary.git' + '@' + '608ded739e036a3aa69db47ac43777dcee506f8e',

    'src/third_party/lss':
      Var('chromium_git') + '/external/linux-syscall-support/lss.git' + '@' + Var('lss_revision'),

    'src/third_party/requests/src':
      Var('chromium_git') + '/external/github.com/kennethreitz/requests.git' + '@' + 'f172b30356d821d180fa4ecfa3e71c7274a32de4',

    'src/third_party/custom_tabs_client/src':
      Var('chromium_git') + '/external/github.com/GoogleChrome/custom-tabs-client.git' + '@' + '8ae46d26e739899d2e35f462beeb20e9c194d0ab',
  },
}


include_rules = [
  # Everybody can use some things.
  # NOTE: THIS HAS TO STAY IN SYNC WITH third_party/DEPS which disallows these.
  '+base',
  '+build',
  '+ipc',

  # Everybody can use headers generated by tools/generate_library_loader.
  '+library_loaders',

  '+testing',
  '+third_party/icu/source/common/unicode',
  '+third_party/icu/source/i18n/unicode',
  '+url',
]


# checkdeps.py shouldn't check include paths for files in these dirs:
skip_child_includes = [
  'breakpad',
  'native_client_sdk',
  'out',
  'sdch',
  'skia',
  'testing',
  'v8',
  'win8',
]


hooks = [
  {
    # This clobbers when necessary (based on get_landmines.py). It must be the
    # first hook so that other things that get/generate into the output
    # directory will not subsequently be clobbered.
    'name': 'landmines',
    'pattern': '.',
    'action': [
        'python',
        'src/build/landmines.py',
    ],
  },
  {
    # Ensure that while generating dependencies lists in .gyp files we don't
    # accidentally reference any .pyc files whose corresponding .py files have
    # already been deleted.
    # We should actually try to avoid generating .pyc files, crbug.com/500078.
    'name': 'remove_stale_pyc_files',
    'pattern': '.',
    'action': [
        'python',
        'src/tools/remove_stale_pyc_files.py',
        'src/android_webview/tools',
        'src/build/android',
        'src/gpu/gles2_conform_support',
        'src/infra',
        'src/ppapi',
        'src/printing',
        'src/third_party/catapult',
        'src/third_party/closure_compiler/build',
        'src/tools',
    ],
  },
  {
    # This downloads binaries for Native Client's newlib toolchain.
    # Done in lieu of building the toolchain from scratch as it can take
    # anywhere from 30 minutes to 4 hours depending on platform to build.
    'name': 'nacltools',
    'pattern': '.',
    'action': [
        'python',
        'src/build/download_nacl_toolchains.py',
        '--mode', 'nacl_core_sdk',
        'sync', '--extract',
    ],
  },
  {
    # This downloads SDK extras and puts them in the
    # third_party/android_tools/sdk/extras directory.
    'name': 'sdkextras',
    'pattern': '.',
    # When adding a new sdk extras package to download, add the package
    # directory and zip file to .gitignore in third_party/android_tools.
    'action': ['python',
               'src/build/android/play_services/update.py',
               'download'
    ],
  },
  {
    # Downloads the current stable linux sysroot to build/linux/ if needed.
    # This sysroot updates at about the same rate that the chrome build deps
    # change. This script is a no-op except for linux users who are doing
    # official chrome builds or cross compiling.
    'name': 'sysroot',
    'pattern': '.',
    'action': ['python', 'src/build/linux/sysroot_scripts/install-sysroot.py',
               '--running-as-hook'],
  },
  {
    # Update the Windows toolchain if necessary.
    'name': 'win_toolchain',
    'pattern': '.',
    'action': ['python', 'src/build/vs_toolchain.py', 'update'],
  },
  # Pull binutils for linux, enabled debug fission for faster linking /
  # debugging when used with clang on Ubuntu Precise.
  # https://code.google.com/p/chromium/issues/detail?id=352046
  {
    'name': 'binutils',
    'pattern': 'src/third_party/binutils',
    'action': [
        'python',
        'src/third_party/binutils/download.py',
    ],
  },
  {
    # Pull clang if needed or requested via GYP_DEFINES.
    # Note: On Win, this should run after win_toolchain, as it may use it.
    'name': 'clang',
    'pattern': '.',
    'action': ['python', 'src/tools/clang/scripts/update.py', '--if-needed'],
  },
  {
    # Update LASTCHANGE.
    'name': 'lastchange',
    'pattern': '.',
    'action': ['python', 'src/build/util/lastchange.py',
               '-o', 'src/build/util/LASTCHANGE'],
  },
  {
    # Update LASTCHANGE.blink.
    'name': 'lastchange_blink',
    'pattern': '.',
    'action': ['python', 'src/build/util/lastchange.py',
               '--git-hash-only',
               '-s', 'src/third_party/WebKit',
               '-o', 'src/build/util/LASTCHANGE.blink'],
  },
  # Pull GN binaries. This needs to be before running GYP below.
  {
    'name': 'gn_win',
    'pattern': '.',
    'action': [ 'download_from_google_storage',
                '--no_resume',
                '--platform=win32',
                '--no_auth',
                '--bucket', 'chromium-gn',
                '-s', 'src/buildtools/win/gn.exe.sha1',
    ],
  },
  {
    'name': 'gn_mac',
    'pattern': '.',
    'action': [ 'download_from_google_storage',
                '--no_resume',
                '--platform=darwin',
                '--no_auth',
                '--bucket', 'chromium-gn',
                '-s', 'src/buildtools/mac/gn.sha1',
    ],
  },
  {
    'name': 'gn_linux64',
    'pattern': '.',
    'action': [ 'download_from_google_storage',
                '--no_resume',
                '--platform=linux*',
                '--no_auth',
                '--bucket', 'chromium-gn',
                '-s', 'src/buildtools/linux64/gn.sha1',
    ],
  },
  # Pull clang-format binaries using checked-in hashes.
  {
    'name': 'clang_format_win',
    'pattern': '.',
    'action': [ 'download_from_google_storage',
                '--no_resume',
                '--platform=win32',
                '--no_auth',
                '--bucket', 'chromium-clang-format',
                '-s', 'src/buildtools/win/clang-format.exe.sha1',
    ],
  },
  {
    'name': 'clang_format_mac',
    'pattern': '.',
    'action': [ 'download_from_google_storage',
                '--no_resume',
                '--platform=darwin',
                '--no_auth',
                '--bucket', 'chromium-clang-format',
                '-s', 'src/buildtools/mac/clang-format.sha1',
    ],
  },
  {
    'name': 'clang_format_linux',
    'pattern': '.',
    'action': [ 'download_from_google_storage',
                '--no_resume',
                '--platform=linux*',
                '--no_auth',
                '--bucket', 'chromium-clang-format',
                '-s', 'src/buildtools/linux64/clang-format.sha1',
    ],
  },
  # Pull the prebuilt libc++ static library for mac.
  {
    'name': 'libcpp_mac',
    'pattern': '.',
    'action': [ 'download_from_google_storage',
                '--no_resume',
                '--platform=darwin',
                '--no_auth',
                '--bucket', 'chromium-libcpp',
                '-s', 'src/third_party/libc++-static/libc++.a.sha1',
    ],
  },
  # Pull luci-go binaries (isolate, swarming) using checked-in hashes.
  {
    'name': 'luci-go_win',
    'pattern': '.',
    'action': [ 'download_from_google_storage',
                '--no_resume',
                '--platform=win32',
                '--no_auth',
                '--bucket', 'chromium-luci',
                '-d', 'src/tools/luci-go/win64',
    ],
  },
  {
    'name': 'luci-go_mac',
    'pattern': '.',
    'action': [ 'download_from_google_storage',
                '--no_resume',
                '--platform=darwin',
                '--no_auth',
                '--bucket', 'chromium-luci',
                '-d', 'src/tools/luci-go/mac64',
    ],
  },
  {
    'name': 'luci-go_linux',
    'pattern': '.',
    'action': [ 'download_from_google_storage',
                '--no_resume',
                '--platform=linux*',
                '--no_auth',
                '--bucket', 'chromium-luci',
                '-d', 'src/tools/luci-go/linux64',
    ],
  },
  # Pull eu-strip binaries using checked-in hashes.
  {
    'name': 'eu-strip',
    'pattern': '.',
    'action': [ 'download_from_google_storage',
                '--no_resume',
                '--platform=linux*',
                '--no_auth',
                '--bucket', 'chromium-eu-strip',
                '-s', 'src/build/linux/bin/eu-strip.sha1',
    ],
  },
  {
    'name': 'drmemory',
    'pattern': '.',
    'action': [ 'download_from_google_storage',
                '--no_resume',
                '--platform=win32',
                '--no_auth',
                '--bucket', 'chromium-drmemory',
                '-s', 'src/third_party/drmemory/drmemory-windows-sfx.exe.sha1',
              ],
  },
  # Pull the Syzygy binaries, used for optimization and instrumentation.
  {
    'name': 'syzygy-binaries',
    'pattern': '.',
    'action': ['python',
               'src/build/get_syzygy_binaries.py',
               '--output-dir=src/third_party/syzygy/binaries',
               '--revision=311374dcf60389071ec231b82bbc61b1fe3c82c1',
               '--overwrite',
    ],
  },
  # TODO(pmonette): Move include files out of binaries folder.
  {
    'name': 'kasko',
    'pattern': '.',
    'action': ['python',
               'src/build/get_syzygy_binaries.py',
               '--output-dir=src/third_party/kasko/binaries',
               '--revision=266a18d9209be5ca5c5dcd0620942b82a2d238f3',
               '--resource=kasko.zip',
               '--resource=kasko_symbols.zip',
               '--overwrite',
    ],
  },
  {
    'name': 'apache_win32',
    'pattern': '\\.sha1',
    'action': [ 'download_from_google_storage',
                '--no_resume',
                '--platform=win32',
                '--directory',
                '--recursive',
                '--no_auth',
                '--num_threads=16',
                '--bucket', 'chromium-apache-win32',
                'src/third_party/apache-win32',
    ],
  },
  {
    'name': 'blimp_fonts',
    'pattern': '.',
    'action': [ 'download_from_google_storage',
                '--no_resume',
                '--platform=linux*',
                '--extract',
                '--no_auth',
                '--bucket', 'chromium-fonts',
                '-s', 'src/third_party/blimp_fonts/font_bundle.tar.gz.sha1',
    ],
  },
  {
    # Pull sanitizer-instrumented third-party libraries if requested via
    # GYP_DEFINES.
    'name': 'instrumented_libraries',
    'pattern': '\\.sha1',
    'action': ['python', 'src/third_party/instrumented_libraries/scripts/download_binaries.py'],
  },
  {
    # A change to a .gyp, .gypi, or to GYP itself should run the generator.
    'name': 'gyp',
    'pattern': '.',
    'action': ['python', 'src/build/gyp_chromium'],
  },
]
