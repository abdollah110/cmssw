import FWCore.ParameterSet.Config as cms
from PhysicsTools.PatAlgos.slimming.primaryVertexAssociation_cfi import primaryVertexAssociation

lostTracks = cms.EDProducer("PATLostTracks",
    inputCandidates = cms.InputTag("particleFlow"),
    packedPFCandidates	= cms.InputTag("packedPFCandidates"),
    inputTracks = cms.InputTag("generalTracks"),
    secondaryVertices = cms.InputTag("inclusiveSecondaryVertices"),
    kshorts=cms.InputTag("generalV0Candidates","Kshort"),
    lambdas=cms.InputTag("generalV0Candidates","Lambda"),
    primaryVertices = cms.InputTag("offlineSlimmedPrimaryVertices"),
    originalVertices = cms.InputTag("offlinePrimaryVertices"),
    muons = cms.InputTag("muons"),
    minPt = cms.double(0.5),
    minHits = cms.uint32(8),
    minPixelHits = cms.uint32(1),
    covarianceVersion = cms.int32(0), #so far: 0 is Phase0, 1 is Phase1
    #covariancePackingSchemas = cms.vint32(1,257,513,769,0),  # a cheaper schema in kb/ev
    covariancePackingSchemas = cms.vint32(8,264,520,776,0),  # more accurate schema
    qualsToAutoAccept = cms.vstring("highPurity"),
    minPtToStoreProps = cms.double(0.95),
    passThroughCut = cms.string("pt>2"),
    pvAssignment = primaryVertexAssociation.assignment
)
from Configuration.Eras.Modifier_phase1Pixel_cff import phase1Pixel
phase1Pixel.toModify(lostTracks, covarianceVersion =1 )

