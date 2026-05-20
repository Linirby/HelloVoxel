SC = glslc

VERT_SHADERS = $(wildcard *.vert)
VERT_SPVS    = $(VERT_SHADERS:.vert=.vert.spv)

FRAG_SHADERS = $(wildcard *.frag)
FRAG_SPVS    = $(FRAG_SHADERS:.frag=.frag.spv)

all: $(VERT_SPVS) $(FRAG_SPVS)

%.vert.spv: %.vert
	$(SC) $< -o $@

%.frag.spv: %.frag
	$(SC) $< -o $@

clean:
	rm -rf *.spv
