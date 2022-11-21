<?xml version="1.0" encoding="ISO-8859-1"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:diff="http://www.via.ecp.fr/~remi/soft/xml/xmldiff" >

<xsl:param name="filter" select="'*'" />

<xsl:include href="helper-status-icons.xsl" />

<xsl:template match="/">
 <html>
 <head><title>Difference lists</title></head>
 <body>
 <h1>Added</h1>
 <table>
 <tr><th>Id</th><th>Value</th></tr>
 <xsl:apply-templates select="//*[@diff:status='added']" />
 </table>
 <h1>Modified</h1>
 <table>
 <tr><th>Id</th><th>Value</th></tr>
 <xsl:apply-templates select="//*[@diff:status='added']" />
 <xsl:apply-templates select="//*[@diff:status='modified']" />
 </table>
 <h1>Removed</h1>
  <table>
 <tr><th>Id</th><th>Value</th></tr>
 <xsl:apply-templates select="//*[@diff:status='added']" />
 <xsl:apply-templates select="//*[@diff:status='removed']" />
 </table>
 </body>
 </html>
</xsl:template>

<xsl:template match="*">
<tr>
 <td><xsl:value-of select="@id" /></td>
 <td><xsl:value-of select="@value" /></td>
</tr>
</xsl:template>

</xsl:stylesheet>