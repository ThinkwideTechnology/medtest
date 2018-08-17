Release Process
====================

* update translations (ping wumpus, Diapolo or tcatm on IRC)
* see https://github.com/medexcoin-project/medexcoin/blob/master/doc/translation_process.md#syncing-with-transifex

* * *

###update (commit) version in sources

	contrib/verifysfbinaries/verify.sh
	doc/README*
	share/setup.nsi
	src/clientversion.h (change CLIENT_VERSION_IS_RELEASE to true)

###tag version in git

	git tag -s v(new version, e.g. 2.0.0.0)

###write release notes. git shortlog helps a lot, for example:

	git shortlog --no-merges v(current version, e.g. 1.1.0.2)..v(new version, e.g. 2.0.0.0)

* * *

###update gitian

 In order to take advantage of the new caching features in gitian, be sure to update to a recent version (e9741525c or higher is recommended)

###perform gitian builds

 From a directory containing the medexcoin source, gitian-builder and gitian.sigs

	export SIGNER=(your gitian key)
	export VERSION=(new version, e.g. 2.0.0.0)
	pushd ./medexcoin
	git checkout v${VERSION}
	popd
	pushd ./gitian-builder

###fetch and build inputs: (first time, or when dependency versions change)
 
	mkdir -p inputs

 Register and download the Apple SDK: (see OSX Readme for details)
 
 https://developer.apple.com/downloads/download.action?path=Developer_Tools/xcode_4.6.3/xcode4630916281a.dmg
 
 Using a Mac, create a tarball for the 10.7 SDK and copy it to the inputs directory:
 
	tar -C /Volumes/Xcode/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/ -czf MacOSX10.7.sdk.tar.gz MacOSX10.7.sdk

 Or you can download it from our website;
 
 	http://medexcoin-project.com/files/sdk/MacOSX10.7.sdk.tar.gz
 	
 If you will be building the RPi2 binary as well, you will need this file in 'gitian-builder/inputs' folder
 
 	http://medexcoin-project.com/files/sdk/raspberrypi-tools.tar.gz
 	
###Optional: Seed the Gitian sources cache

  By default, gitian will fetch source files as needed. For offline builds, they can be fetched ahead of time:

	make -C ../medexcoin/depends download SOURCES_PATH=`pwd`/cache/common

  Only missing files will be fetched, so this is safe to re-run for each build.

###Build Medexcoin Core for Linux, Windows, and OS X:

	./bin/gbuild --commit medexcoin=v${VERSION} ../medexcoin/contrib/gitian-descriptors/gitian-linux.yml
	./bin/gsign --signer $SIGNER --release ${VERSION}-linux --destination ../gitian.sigs/ ../medexcoin/contrib/gitian-descriptors/gitian-linux.yml
	mv build/out/medexcoin-*.tar.gz build/out/src/medexcoin-*.tar.gz ../
	./bin/gbuild --commit medexcoin=v${VERSION} ../medexcoin/contrib/gitian-descriptors/gitian-win.yml
	./bin/gsign --signer $SIGNER --release ${VERSION}-win --destination ../gitian.sigs/ ../medexcoin/contrib/gitian-descriptors/gitian-win.yml
	mv build/out/medexcoin-*.zip build/out/medexcoin-*.exe ../
	./bin/gbuild --commit medexcoin=v${VERSION} ../medexcoin/contrib/gitian-descriptors/gitian-osx.yml
	./bin/gsign --signer $SIGNER --release ${VERSION}-osx-unsigned --destination ../gitian.sigs/ ../medexcoin/contrib/gitian-descriptors/gitian-osx.yml
	mv build/out/medexcoin-*-unsigned.tar.gz inputs/medexcoin-osx-unsigned.tar.gz
	mv build/out/medexcoin-*.tar.gz build/out/medexcoin-*.dmg ../
	popd
  Build output expected:

  1. source tarball (medexcoin-${VERSION}.tar.gz)
  2. linux 32-bit and 64-bit binaries dist tarballs (medexcoin-${VERSION}-linux[32|64].tar.gz)
  3. windows 32-bit and 64-bit installers and dist zips (medexcoin-${VERSION}-win[32|64]-setup.exe, medexcoin-${VERSION}-win[32|64].zip)
  4. OSX unsigned installer (medexcoin-${VERSION}-osx-unsigned.dmg)
  5. Gitian signatures (in gitian.sigs/${VERSION}-<linux|win|osx-unsigned>/(your gitian key)/

###Next steps:

Commit your signature to gitian.sigs:

	pushd gitian.sigs
	git add ${VERSION}-linux/${SIGNER}
	git add ${VERSION}-win/${SIGNER}
	git add ${VERSION}-osx-unsigned/${SIGNER}
	git commit -a
	git push  # Assuming you can push to the gitian.sigs tree
	popd

  Wait for OSX detached signature:
	Once the OSX build has 3 matching signatures ***TODO*** will sign it with the apple App-Store key.
	He will then upload a detached signature to be combined with the unsigned app to create a signed binary.

  Create the signed OSX binary:

	pushd ./gitian-builder
	# Fetch the signature as instructed by Evan
	cp signature.tar.gz inputs/
	./bin/gbuild -i ../medexcoin/contrib/gitian-descriptors/gitian-osx-signer.yml
	./bin/gsign --signer $SIGNER --release ${VERSION}-osx-signed --destination ../gitian.sigs/ ../medexcoin/contrib/gitian-descriptors/gitian-osx-signer.yml
	mv build/out/medexcoin-osx-signed.dmg ../medexcoin-${VERSION}-osx.dmg
	popd

Commit your signature for the signed OSX binary:

	pushd gitian.sigs
	git add ${VERSION}-osx-signed/${SIGNER}
	git commit -a
	git push  # Assuming you can push to the gitian.sigs tree
	popd

-------------------------------------------------------------------------

### After 3 or more people have gitian-built and their results match:

- Perform code-signing.

    - Code-sign Windows -setup.exe (in a Windows virtual machine using signtool)

- Create `SHA256SUMS.asc` for the builds, and GPG-sign it:
```bash
sha256sum * > SHA256SUMS
gpg --digest-algo sha256 --clearsign SHA256SUMS # outputs SHA256SUMS.asc
rm SHA256SUMS
```
(the digest algorithm is forced to sha256 to avoid confusion of the `Hash:` header that GPG adds with the SHA256 used for the files)

- Upload zips and installers, as well as `SHA256SUMS.asc` from last step, to the bitcoin.org server
  into `/var/www/bin/bitcoin-core-${VERSION}`

- Add release notes for the new version to the directory `doc/release-notes` in git master

- Celebrate
