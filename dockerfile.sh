CONTAINER=cpctools/cpctools

case $1 in
	build)
		docker build  -t $CONTAINER .
		;;
	run)
		docker run -v $(pwd):/cpctools --rm=true $CONTAINER /build/docker-build.sh

esac
