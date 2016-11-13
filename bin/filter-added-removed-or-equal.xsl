<?xml version="1.0" encoding="ISO-8859-1"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:diff="http://www.via.ecp.fr/~remi/soft/xml/xmldiff" >

<xsl:template match="/">
 <xsl:apply-templates />
</xsl:template>

<xsl:template match="*">
 <node>
   <xsl:call-template name="icon" />
  <xsl:attribute name="children">*[not(@diff:status)]|*[@diff:status='added']|*[@diff:status='removed']|*[count(descendant::*[not(@diff:status) or @diff:status='added' or @diff:status='removed'])&gt;0]</xsl:attribute>
  <xsl:value-of select="name()" />
  <xsl:for-each select="@*[name()!='diff:status']">
   <xsl:text> </xsl:text><xsl:value-of select="name()" /><xsl:text>="</xsl:text><xsl:value-of select="." /><xsl:text>"</xsl:text>
  </xsl:for-each>
  <xsl:if test="count(text()) &gt; 0"><xsl:text> = </xsl:text><xsl:value-of select="text()" /></xsl:if>
  </node>
</xsl:template>

<xsl:template name="icon">
  <xsl:if test="@diff:status">
  <xsl:attribute name="icon">
   <xsl:choose>
    <xsl:when test="@diff:status='added'">added</xsl:when>
    <xsl:when test="@diff:status='removed'">removed</xsl:when>
    <xsl:when test="@diff:status='modified'">modified</xsl:when>
    <xsl:when test="@diff:status='below'">question</xsl:when>
    <xsl:otherwise>unknown</xsl:otherwise>
   </xsl:choose>
  </xsl:attribute>
  </xsl:if>
</xsl:template>    

</xsl:stylesheet>