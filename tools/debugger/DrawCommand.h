/*
 * Copyright 2012 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef SKDRAWCOMMAND_H_
#define SKDRAWCOMMAND_H_

#include "SkBitmap.h"
#include "SkCanvas.h"
#include "SkDrawShadowInfo.h"
#include "SkFlattenable.h"
#include "SkJSONWriter.h"
#include "SkPath.h"
#include "SkRRect.h"
#include "SkRSXform.h"
#include "SkRegion.h"
#include "SkString.h"
#include "SkTDArray.h"
#include "SkTLazy.h"
#include "SkVertices.h"
#include "UrlDataManager.h"

class DrawCommand {
public:
    enum OpType {
        kBeginDrawPicture_OpType,
        kClear_OpType,
        kClipPath_OpType,
        kClipRegion_OpType,
        kClipRect_OpType,
        kClipRRect_OpType,
        kConcat_OpType,
        kDrawAnnotation_OpType,
        kDrawBitmap_OpType,
        kDrawBitmapLattice_OpType,
        kDrawBitmapNine_OpType,
        kDrawBitmapRect_OpType,
        kDrawDRRect_OpType,
        kDrawImage_OpType,
        kDrawImageLattice_OpType,
        kDrawImageNine_OpType,
        kDrawImageRect_OpType,
        kDrawOval_OpType,
        kDrawArc_OpType,
        kDrawPaint_OpType,
        kDrawPatch_OpType,
        kDrawPath_OpType,
        kDrawPoints_OpType,
        kDrawRect_OpType,
        kDrawRRect_OpType,
        kDrawRegion_OpType,
        kDrawShadow_OpType,
        kDrawTextBlob_OpType,
        kDrawVertices_OpType,
        kDrawAtlas_OpType,
        kDrawDrawable_OpType,
        kDrawEdgeAAQuad_OpType,
        kDrawEdgeAAImageSet_OpType,
        kEndDrawPicture_OpType,
        kRestore_OpType,
        kSave_OpType,
        kSaveLayer_OpType,
        kSetMatrix_OpType,

        kLast_OpType = kSetMatrix_OpType
    };

    static const int kOpTypeCount = kLast_OpType + 1;

    static void WritePNG(SkBitmap bitmap, SkWStream& out);

    DrawCommand(OpType opType);

    virtual ~DrawCommand() {}

    bool isVisible() const { return fVisible; }

    void setVisible(bool toggle) { fVisible = toggle; }

    virtual void execute(SkCanvas*) const = 0;

    virtual bool render(SkCanvas* canvas) const { return false; }

    virtual void toJSON(SkJSONWriter& writer, UrlDataManager& urlDataManager) const;

    static const char* GetCommandString(OpType type);

    // Helper methods for converting things to JSON
    static void MakeJsonColor(SkJSONWriter&, const SkColor color);
    static void MakeJsonColor4f(SkJSONWriter&, const SkColor4f& color);
    static void MakeJsonPoint(SkJSONWriter&, const SkPoint& point);
    static void MakeJsonPoint(SkJSONWriter&, SkScalar x, SkScalar y);
    static void MakeJsonPoint3(SkJSONWriter&, const SkPoint3& point);
    static void MakeJsonRect(SkJSONWriter&, const SkRect& rect);
    static void MakeJsonIRect(SkJSONWriter&, const SkIRect&);
    static void MakeJsonMatrix(SkJSONWriter&, const SkMatrix&);
    static void MakeJsonPath(SkJSONWriter&, const SkPath& path);
    static void MakeJsonRegion(SkJSONWriter&, const SkRegion& region);
    static void MakeJsonPaint(SkJSONWriter&, const SkPaint& paint, UrlDataManager& urlDataManager);
    static void MakeJsonLattice(SkJSONWriter&, const SkCanvas::Lattice& lattice);

    static void flatten(const SkFlattenable* flattenable,
                        SkJSONWriter&        writer,
                        UrlDataManager&      urlDataManager);
    static bool flatten(const SkImage& image, SkJSONWriter& writer, UrlDataManager& urlDataManager);
    static bool flatten(const SkBitmap& bitmap,
                        SkJSONWriter&   writer,
                        UrlDataManager& urlDataManager);

private:
    OpType fOpType;
    bool   fVisible;
};

class SkRestoreCommand : public DrawCommand {
public:
    SkRestoreCommand();
    void execute(SkCanvas* canvas) const override;

private:
    typedef DrawCommand INHERITED;
};

class SkClearCommand : public DrawCommand {
public:
    SkClearCommand(SkColor color);
    void execute(SkCanvas* canvas) const override;
    void toJSON(SkJSONWriter& writer, UrlDataManager& urlDataManager) const override;

private:
    SkColor fColor;

    typedef DrawCommand INHERITED;
};

class SkClipPathCommand : public DrawCommand {
public:
    SkClipPathCommand(const SkPath& path, SkClipOp op, bool doAA);
    void execute(SkCanvas* canvas) const override;
    bool render(SkCanvas* canvas) const override;
    void toJSON(SkJSONWriter& writer, UrlDataManager& urlDataManager) const override;

private:
    SkPath   fPath;
    SkClipOp fOp;
    bool     fDoAA;

    typedef DrawCommand INHERITED;
};

class SkClipRegionCommand : public DrawCommand {
public:
    SkClipRegionCommand(const SkRegion& region, SkClipOp op);
    void execute(SkCanvas* canvas) const override;
    void toJSON(SkJSONWriter& writer, UrlDataManager& urlDataManager) const override;

private:
    SkRegion fRegion;
    SkClipOp fOp;

    typedef DrawCommand INHERITED;
};

class SkClipRectCommand : public DrawCommand {
public:
    SkClipRectCommand(const SkRect& rect, SkClipOp op, bool doAA);
    void execute(SkCanvas* canvas) const override;
    void toJSON(SkJSONWriter& writer, UrlDataManager& urlDataManager) const override;

private:
    SkRect   fRect;
    SkClipOp fOp;
    bool     fDoAA;

    typedef DrawCommand INHERITED;
};

class SkClipRRectCommand : public DrawCommand {
public:
    SkClipRRectCommand(const SkRRect& rrect, SkClipOp op, bool doAA);
    void execute(SkCanvas* canvas) const override;
    bool render(SkCanvas* canvas) const override;
    void toJSON(SkJSONWriter& writer, UrlDataManager& urlDataManager) const override;

private:
    SkRRect  fRRect;
    SkClipOp fOp;
    bool     fDoAA;

    typedef DrawCommand INHERITED;
};

class SkConcatCommand : public DrawCommand {
public:
    SkConcatCommand(const SkMatrix& matrix);
    void execute(SkCanvas* canvas) const override;
    void toJSON(SkJSONWriter& writer, UrlDataManager& urlDataManager) const override;

private:
    SkMatrix fMatrix;

    typedef DrawCommand INHERITED;
};

class SkDrawAnnotationCommand : public DrawCommand {
public:
    SkDrawAnnotationCommand(const SkRect&, const char key[], sk_sp<SkData> value);
    void execute(SkCanvas* canvas) const override;
    void toJSON(SkJSONWriter& writer, UrlDataManager& urlDataManager) const override;

private:
    SkRect        fRect;
    SkString      fKey;
    sk_sp<SkData> fValue;

    typedef DrawCommand INHERITED;
};

class SkDrawBitmapCommand : public DrawCommand {
public:
    SkDrawBitmapCommand(const SkBitmap& bitmap, SkScalar left, SkScalar top, const SkPaint* paint);
    void execute(SkCanvas* canvas) const override;
    bool render(SkCanvas* canvas) const override;
    void toJSON(SkJSONWriter& writer, UrlDataManager& urlDataManager) const override;

private:
    SkBitmap         fBitmap;
    SkScalar         fLeft;
    SkScalar         fTop;
    SkTLazy<SkPaint> fPaint;

    typedef DrawCommand INHERITED;
};

class SkDrawBitmapLatticeCommand : public DrawCommand {
public:
    SkDrawBitmapLatticeCommand(const SkBitmap&          bitmap,
                               const SkCanvas::Lattice& lattice,
                               const SkRect&            dst,
                               const SkPaint*           paint);
    void execute(SkCanvas* canvas) const override;
    bool render(SkCanvas* canvas) const override;
    void toJSON(SkJSONWriter& writer, UrlDataManager& urlDataManager) const override;

private:
    SkBitmap          fBitmap;
    SkCanvas::Lattice fLattice;
    SkRect            fDst;
    SkTLazy<SkPaint>  fPaint;

    typedef DrawCommand INHERITED;
};

class SkDrawBitmapNineCommand : public DrawCommand {
public:
    SkDrawBitmapNineCommand(const SkBitmap& bitmap,
                            const SkIRect&  center,
                            const SkRect&   dst,
                            const SkPaint*  paint);
    void execute(SkCanvas* canvas) const override;
    bool render(SkCanvas* canvas) const override;
    void toJSON(SkJSONWriter& writer, UrlDataManager& urlDataManager) const override;

private:
    SkBitmap         fBitmap;
    SkIRect          fCenter;
    SkRect           fDst;
    SkTLazy<SkPaint> fPaint;

    typedef DrawCommand INHERITED;
};

class SkDrawBitmapRectCommand : public DrawCommand {
public:
    SkDrawBitmapRectCommand(const SkBitmap& bitmap,
                            const SkRect*   src,
                            const SkRect&   dst,
                            const SkPaint*  paint,
                            SkCanvas::SrcRectConstraint);
    void execute(SkCanvas* canvas) const override;
    bool render(SkCanvas* canvas) const override;
    void toJSON(SkJSONWriter& writer, UrlDataManager& urlDataManager) const override;

private:
    SkBitmap                    fBitmap;
    SkTLazy<SkRect>             fSrc;
    SkRect                      fDst;
    SkTLazy<SkPaint>            fPaint;
    SkCanvas::SrcRectConstraint fConstraint;

    typedef DrawCommand INHERITED;
};

class SkDrawImageCommand : public DrawCommand {
public:
    SkDrawImageCommand(const SkImage* image, SkScalar left, SkScalar top, const SkPaint* paint);
    void execute(SkCanvas* canvas) const override;
    bool render(SkCanvas* canvas) const override;
    void toJSON(SkJSONWriter& writer, UrlDataManager& urlDataManager) const override;

private:
    sk_sp<const SkImage> fImage;
    SkScalar             fLeft;
    SkScalar             fTop;
    SkTLazy<SkPaint>     fPaint;

    typedef DrawCommand INHERITED;
};

class SkDrawImageLatticeCommand : public DrawCommand {
public:
    SkDrawImageLatticeCommand(const SkImage*           image,
                              const SkCanvas::Lattice& lattice,
                              const SkRect&            dst,
                              const SkPaint*           paint);
    void execute(SkCanvas* canvas) const override;
    bool render(SkCanvas* canvas) const override;
    void toJSON(SkJSONWriter& writer, UrlDataManager& urlDataManager) const override;

private:
    sk_sp<const SkImage> fImage;
    SkCanvas::Lattice    fLattice;
    SkRect               fDst;
    SkTLazy<SkPaint>     fPaint;

    typedef DrawCommand INHERITED;
};

class SkDrawImageNineCommand : public DrawCommand {
public:
    SkDrawImageNineCommand(const SkImage* image,
                           const SkIRect& center,
                           const SkRect&  dst,
                           const SkPaint* paint);
    void execute(SkCanvas* canvas) const override;
    bool render(SkCanvas* canvas) const override;
    void toJSON(SkJSONWriter& writer, UrlDataManager& urlDataManager) const override;

private:
    sk_sp<const SkImage> fImage;
    SkIRect              fCenter;
    SkRect               fDst;
    SkTLazy<SkPaint>     fPaint;

    typedef DrawCommand INHERITED;
};

class SkDrawImageRectCommand : public DrawCommand {
public:
    SkDrawImageRectCommand(const SkImage*              image,
                           const SkRect*               src,
                           const SkRect&               dst,
                           const SkPaint*              paint,
                           SkCanvas::SrcRectConstraint constraint);
    void execute(SkCanvas* canvas) const override;
    bool render(SkCanvas* canvas) const override;
    void toJSON(SkJSONWriter& writer, UrlDataManager& urlDataManager) const override;

private:
    sk_sp<const SkImage>        fImage;
    SkTLazy<SkRect>             fSrc;
    SkRect                      fDst;
    SkTLazy<SkPaint>            fPaint;
    SkCanvas::SrcRectConstraint fConstraint;

    typedef DrawCommand INHERITED;
};

class SkDrawOvalCommand : public DrawCommand {
public:
    SkDrawOvalCommand(const SkRect& oval, const SkPaint& paint);
    void execute(SkCanvas* canvas) const override;
    bool render(SkCanvas* canvas) const override;
    void toJSON(SkJSONWriter& writer, UrlDataManager& urlDataManager) const override;

private:
    SkRect  fOval;
    SkPaint fPaint;

    typedef DrawCommand INHERITED;
};

class SkDrawArcCommand : public DrawCommand {
public:
    SkDrawArcCommand(const SkRect&  oval,
                     SkScalar       startAngle,
                     SkScalar       sweepAngle,
                     bool           useCenter,
                     const SkPaint& paint);
    void execute(SkCanvas* canvas) const override;
    bool render(SkCanvas* canvas) const override;
    void toJSON(SkJSONWriter& writer, UrlDataManager& urlDataManager) const override;

private:
    SkRect   fOval;
    SkScalar fStartAngle;
    SkScalar fSweepAngle;
    bool     fUseCenter;
    SkPaint  fPaint;

    typedef DrawCommand INHERITED;
};

class SkDrawPaintCommand : public DrawCommand {
public:
    SkDrawPaintCommand(const SkPaint& paint);
    void execute(SkCanvas* canvas) const override;
    bool render(SkCanvas* canvas) const override;
    void toJSON(SkJSONWriter& writer, UrlDataManager& urlDataManager) const override;

private:
    SkPaint fPaint;

    typedef DrawCommand INHERITED;
};

class SkDrawPathCommand : public DrawCommand {
public:
    SkDrawPathCommand(const SkPath& path, const SkPaint& paint);
    void execute(SkCanvas* canvas) const override;
    bool render(SkCanvas* canvas) const override;
    void toJSON(SkJSONWriter& writer, UrlDataManager& urlDataManager) const override;

private:
    SkPath  fPath;
    SkPaint fPaint;

    typedef DrawCommand INHERITED;
};

class SkBeginDrawPictureCommand : public DrawCommand {
public:
    SkBeginDrawPictureCommand(const SkPicture* picture,
                              const SkMatrix*  matrix,
                              const SkPaint*   paint);

    void execute(SkCanvas* canvas) const override;
    bool render(SkCanvas* canvas) const override;

private:
    sk_sp<const SkPicture> fPicture;
    SkTLazy<SkMatrix>      fMatrix;
    SkTLazy<SkPaint>       fPaint;

    typedef DrawCommand INHERITED;
};

class SkEndDrawPictureCommand : public DrawCommand {
public:
    SkEndDrawPictureCommand(bool restore);

    void execute(SkCanvas* canvas) const override;

private:
    bool fRestore;

    typedef DrawCommand INHERITED;
};

class SkDrawPointsCommand : public DrawCommand {
public:
    SkDrawPointsCommand(SkCanvas::PointMode mode,
                        size_t              count,
                        const SkPoint       pts[],
                        const SkPaint&      paint);
    void execute(SkCanvas* canvas) const override;
    bool render(SkCanvas* canvas) const override;
    void toJSON(SkJSONWriter& writer, UrlDataManager& urlDataManager) const override;

private:
    SkCanvas::PointMode fMode;
    SkTDArray<SkPoint>  fPts;
    SkPaint             fPaint;

    typedef DrawCommand INHERITED;
};

class SkDrawRegionCommand : public DrawCommand {
public:
    SkDrawRegionCommand(const SkRegion& region, const SkPaint& paint);
    void execute(SkCanvas* canvas) const override;
    bool render(SkCanvas* canvas) const override;
    void toJSON(SkJSONWriter& writer, UrlDataManager& urlDataManager) const override;

private:
    SkRegion fRegion;
    SkPaint  fPaint;

    typedef DrawCommand INHERITED;
};

class SkDrawTextBlobCommand : public DrawCommand {
public:
    SkDrawTextBlobCommand(sk_sp<SkTextBlob> blob, SkScalar x, SkScalar y, const SkPaint& paint);

    void execute(SkCanvas* canvas) const override;
    bool render(SkCanvas* canvas) const override;
    void toJSON(SkJSONWriter& writer, UrlDataManager& urlDataManager) const override;

private:
    sk_sp<SkTextBlob> fBlob;
    SkScalar          fXPos;
    SkScalar          fYPos;
    SkPaint           fPaint;

    typedef DrawCommand INHERITED;
};

class SkDrawPatchCommand : public DrawCommand {
public:
    SkDrawPatchCommand(const SkPoint  cubics[12],
                       const SkColor  colors[4],
                       const SkPoint  texCoords[4],
                       SkBlendMode    bmode,
                       const SkPaint& paint);
    void execute(SkCanvas* canvas) const override;
    void toJSON(SkJSONWriter& writer, UrlDataManager& urlDataManager) const override;

private:
    SkPoint     fCubics[12];
    SkColor*    fColorsPtr;
    SkColor     fColors[4];
    SkPoint*    fTexCoordsPtr;
    SkPoint     fTexCoords[4];
    SkBlendMode fBlendMode;
    SkPaint     fPaint;

    typedef DrawCommand INHERITED;
};

class SkDrawRectCommand : public DrawCommand {
public:
    SkDrawRectCommand(const SkRect& rect, const SkPaint& paint);
    void execute(SkCanvas* canvas) const override;
    void toJSON(SkJSONWriter& writer, UrlDataManager& urlDataManager) const override;

private:
    SkRect  fRect;
    SkPaint fPaint;

    typedef DrawCommand INHERITED;
};

class SkDrawRRectCommand : public DrawCommand {
public:
    SkDrawRRectCommand(const SkRRect& rrect, const SkPaint& paint);
    void execute(SkCanvas* canvas) const override;
    bool render(SkCanvas* canvas) const override;
    void toJSON(SkJSONWriter& writer, UrlDataManager& urlDataManager) const override;

private:
    SkRRect fRRect;
    SkPaint fPaint;

    typedef DrawCommand INHERITED;
};

class SkDrawDRRectCommand : public DrawCommand {
public:
    SkDrawDRRectCommand(const SkRRect& outer, const SkRRect& inner, const SkPaint& paint);
    void execute(SkCanvas* canvas) const override;
    bool render(SkCanvas* canvas) const override;
    void toJSON(SkJSONWriter& writer, UrlDataManager& urlDataManager) const override;

private:
    SkRRect fOuter;
    SkRRect fInner;
    SkPaint fPaint;

    typedef DrawCommand INHERITED;
};

class SkDrawVerticesCommand : public DrawCommand {
public:
    SkDrawVerticesCommand(sk_sp<SkVertices>, SkBlendMode, const SkPaint&);

    void execute(SkCanvas* canvas) const override;

private:
    sk_sp<SkVertices> fVertices;
    SkBlendMode       fBlendMode;
    SkPaint           fPaint;

    typedef DrawCommand INHERITED;
};

class SkDrawAtlasCommand : public DrawCommand {
public:
    SkDrawAtlasCommand(const SkImage*,
                       const SkRSXform[],
                       const SkRect[],
                       const SkColor[],
                       int,
                       SkBlendMode,
                       const SkRect*,
                       const SkPaint*);

    void execute(SkCanvas* canvas) const override;

private:
    sk_sp<const SkImage> fImage;
    SkTDArray<SkRSXform> fXform;
    SkTDArray<SkRect>    fTex;
    SkTDArray<SkColor>   fColors;
    SkBlendMode          fBlendMode;
    SkTLazy<SkRect>      fCull;
    SkTLazy<SkPaint>     fPaint;

    typedef DrawCommand INHERITED;
};

class SkSaveCommand : public DrawCommand {
public:
    SkSaveCommand();
    void execute(SkCanvas* canvas) const override;

private:
    typedef DrawCommand INHERITED;
};

class SkSaveLayerCommand : public DrawCommand {
public:
    SkSaveLayerCommand(const SkCanvas::SaveLayerRec&);
    void execute(SkCanvas* canvas) const override;
    void toJSON(SkJSONWriter& writer, UrlDataManager& urlDataManager) const override;

private:
    SkTLazy<SkRect>            fBounds;
    SkTLazy<SkPaint>           fPaint;
    sk_sp<const SkImageFilter> fBackdrop;
    uint32_t                   fSaveLayerFlags;

    typedef DrawCommand INHERITED;
};

class SkSetMatrixCommand : public DrawCommand {
public:
    SkSetMatrixCommand(const SkMatrix& matrix);
    void execute(SkCanvas* canvas) const override;
    void toJSON(SkJSONWriter& writer, UrlDataManager& urlDataManager) const override;

private:
    SkMatrix fMatrix;

    typedef DrawCommand INHERITED;
};

class SkDrawShadowCommand : public DrawCommand {
public:
    SkDrawShadowCommand(const SkPath& path, const SkDrawShadowRec& rec);
    void execute(SkCanvas* canvas) const override;
    bool render(SkCanvas* canvas) const override;
    void toJSON(SkJSONWriter& writer, UrlDataManager& urlDataManager) const override;

private:
    SkPath          fPath;
    SkDrawShadowRec fShadowRec;

    typedef DrawCommand INHERITED;
};

class SkDrawDrawableCommand : public DrawCommand {
public:
    SkDrawDrawableCommand(SkDrawable*, const SkMatrix*);
    void execute(SkCanvas* canvas) const override;

private:
    sk_sp<SkDrawable> fDrawable;
    SkTLazy<SkMatrix> fMatrix;

    typedef DrawCommand INHERITED;
};

class SkDrawEdgeAAQuadCommand : public DrawCommand {
public:
    SkDrawEdgeAAQuadCommand(const SkRect&         rect,
                            const SkPoint         clip[4],
                            SkCanvas::QuadAAFlags aa,
                            SkColor               color,
                            SkBlendMode           mode);
    void execute(SkCanvas* canvas) const override;

private:
    SkRect                fRect;
    SkPoint               fClip[4];
    int                   fHasClip;
    SkCanvas::QuadAAFlags fAA;
    SkColor               fColor;
    SkBlendMode           fMode;

    typedef DrawCommand INHERITED;
};

class SkDrawEdgeAAImageSetCommand : public DrawCommand {
public:
    SkDrawEdgeAAImageSetCommand(const SkCanvas::ImageSetEntry[],
                                int count,
                                const SkPoint[],
                                const SkMatrix[],
                                const SkPaint*,
                                SkCanvas::SrcRectConstraint);
    void execute(SkCanvas* canvas) const override;

private:
    SkAutoTArray<SkCanvas::ImageSetEntry> fSet;
    int                                   fCount;
    SkAutoTArray<SkPoint>                 fDstClips;
    SkAutoTArray<SkMatrix>                fPreViewMatrices;
    SkTLazy<SkPaint>                      fPaint;
    SkCanvas::SrcRectConstraint           fConstraint;

    typedef DrawCommand INHERITED;
};
#endif