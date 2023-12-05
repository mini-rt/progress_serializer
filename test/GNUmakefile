.DEFAULT_GOAL := test
.PHONY: test

test: test.exe
	./test.exe

E = =

test.exe: main.c ../lib/base/libminirt_base.precision$E1.target$Edevelopment.profile$Edebug.a ../libminirt_progress_serializer.precision$E1.target$Edevelopment.profile$Edebug.a
	$(CC) -I ../lib/base/include -I ../include -o $@ -L ../lib/base -L .. -l minirt_base.precision$E1.target$Edevelopment.profile$Edebug -l minirt_progress_serializer.precision$E1.target$Edevelopment.profile$Edebug $<

.PHONY: ../lib/base/libminirt_base.precision$E1.target$Edevelopment.profile$Edebug.a
../lib/base/libminirt_base.precision$E1.target$Edevelopment.profile$Edebug.a:
	$(MAKE) -C ../lib/base MINIRT_PRECISION=float TARGET$Edevelopment PROFILE$Edebug libminirt_base.precision$E1.target$Edevelopment.profile$Edebug.a

.PHONY: ../libminirt_progress_serializer.precision$E1.target$Edevelopment.profile$Edebug.a
../libminirt_progress_serializer.precision$E1.target$Edevelopment.profile$Edebug.a:
	$(MAKE) -C .. MINIRT_PRECISION=float TARGET=development PROFILE=debug libminirt_progress_serializer.precision$E1.target$Edevelopment.profile$Edebug.a
